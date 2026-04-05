#pragma once

#include <array>
#include <cstddef>
#include <libkxi/type_list_details/concepts.hpp>
#include <libkxi/type_list_details/fwd.hpp>
#include <type_traits>

namespace kxi::type_list {

template <typename... Args>
struct TypeList {};

template <concepts::TypeList TypeListT>
struct GetSize;

template <typename... Types>
struct GetSize<TypeList<Types...>> {
  static constexpr const std::size_t value = sizeof...(Types);
};

template <concepts::TypeList TypeListT>
constexpr const std::size_t GetSizeV = GetSize<TypeListT>::value;

template <std::size_t I, concepts::TypeList TypeListT>
struct GetType;

template <std::size_t I, typename Head, typename... Tail>
struct GetType<I, TypeList<Head, Tail...>> {
  using type = GetType<I - 1, TypeList<Tail...>>::type;
};

template <typename Head, typename... Tail>
struct GetType<0, TypeList<Head, Tail...>> {
  using type = Head;
};

template <typename T, concepts::TypeList TypeListT>
struct GetCount;

template <typename T, typename Head, typename... Tail>
struct GetCount<T, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      GetCount<T, TypeList<Tail...>>::value;
};

template <typename Head, typename... Tail>
struct GetCount<Head, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      1 + GetCount<Head, TypeList<Tail...>>::value;
};

template <typename Head>
struct GetCount<Head, TypeList<>> {
  static constexpr const std::size_t value = 0;
};

template <typename T, concepts::TypeList TypeListT>
requires(GetCount<T, TypeListT>::value == 1)
struct GetIndex;

template <typename T, typename Head, typename... Tail>
struct GetIndex<T, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      1 + GetIndex<T, TypeList<Tail...>>::value;
};

template <typename Head, typename... Tail>
struct GetIndex<Head, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value = 0;
};

template <std::size_t I, concepts::TypeList TypeListT>
using GetTypeT = typename GetType<I, TypeListT>::type;

template <typename T, concepts::TypeList TypeListT>
constexpr const std::size_t GetIndexV = GetIndex<T, TypeListT>::value;

struct DestinationPos {
  std::size_t list_pos;
  std::size_t elem_pos;
};

template <std::size_t I, concepts::TypeList... TypeListsT>
struct GetDestinationPos {
 private:
  static constexpr DestinationPos GetDestinationPosImpl() {
    static constexpr std::array kListSizes = {GetSizeV<TypeListsT>...};
    std::size_t current_list = 0;
    std::size_t current_pos = I;

    for (const auto& list_size : kListSizes) {
      if (current_pos < list_size) {
        break;
      }
      current_list += 1;
      current_pos -= list_size;
    }

    return {.list_pos = current_list, .elem_pos = current_pos};
  }

 public:
  static constexpr const DestinationPos value = GetDestinationPosImpl();
};

template <std::size_t I, concepts::TypeList... TypeListsT>
constexpr const DestinationPos GetDestinationPosV =
    GetDestinationPos<I, TypeListsT...>::value;

template <concepts::TypeList... TypeListsT>
struct CatLists;

template <typename... CurrTypes, typename... NextTypes,
          concepts::TypeList... NextTypeListsT>
struct CatLists<TypeList<CurrTypes...>, TypeList<NextTypes...>,
                NextTypeListsT...> {
  using type =
      CatLists<TypeList<CurrTypes..., NextTypes...>, NextTypeListsT...>::type;
};

template <typename... CurrTypes>
struct CatLists<TypeList<CurrTypes...>> {
  using type = TypeList<CurrTypes...>;
};

template <concepts::TypeList... TypeListsT>
using CatListsT = CatLists<TypeListsT...>::type;

};  // namespace kxi::type_list