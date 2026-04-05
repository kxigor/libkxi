#pragma once

#include <array>
#include <cstddef>
#include <type_traits>

#include "tuple_details/tuple_fwd.hpp"

namespace kxi::type_list {

template <typename... Args>
struct TypeList {};

template <typename L>
struct GetSize;

template <typename... Types>
struct GetSize<TypeList<Types...>> {
  static constexpr const std::size_t value = sizeof...(Types);
};

template <typename L>
constexpr const std::size_t GetSizeV = GetSize<L>::value;

namespace details {
template <std::size_t I, typename L>
struct GetTypeImpl;

template <std::size_t I, typename Head, typename... Tail>
struct GetTypeImpl<I, TypeList<Head, Tail...>> {
  using type = GetTypeImpl<I - 1, TypeList<Tail...>>::type;
};

template <typename Head, typename... Tail>
struct GetTypeImpl<0, TypeList<Head, Tail...>> {
  using type = Head;
};

template <typename T, typename L>
struct GetCountImpl;

template <typename T, typename Head, typename... Tail>
struct GetCountImpl<T, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      GetCountImpl<T, TypeList<Tail...>>::value;
};

template <typename Head, typename... Tail>
struct GetCountImpl<Head, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      1 + GetCountImpl<Head, TypeList<Tail...>>::value;
};

template <typename Head>
struct GetCountImpl<Head, TypeList<>> {
  static constexpr const std::size_t value = 0;
};

template <typename T, typename L>
struct GetIndexImpl;

template <typename T, typename Head, typename... Tail>
struct GetIndexImpl<T, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value =
      1 + GetIndexImpl<T, TypeList<Tail...>>::value;
};

template <typename Head, typename... Tail>
struct GetIndexImpl<Head, TypeList<Head, Tail...>> {
  static constexpr const std::size_t value = 0;
};

}  // namespace details

template <typename L, std::size_t I>
struct GetType {
  using type = details::GetTypeImpl<I, L>::type;
};

template <typename L, std::size_t I>
using GetTypeT = typename GetType<L, I>::type;

template <typename L, typename T>
requires(details::GetCountImpl<T, L>::value == 1)
struct GetIndex {
  static constexpr const std::size_t value = details::GetIndexImpl<T, L>::value;
};

template <typename L, typename T>
constexpr const std::size_t GetIndexV = GetIndex<L, T>::value;

struct DestinationPos {
  std::size_t list_pos;
  std::size_t elem_pos;
};

template <std::size_t I, typename... Lists>
struct GetDestinationPos {
 private:
  static constexpr DestinationPos GetDestinationPosImpl() {
    static constexpr std::array kListSizes = {GetSizeV<Lists>...};
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

template <std::size_t I, typename... Lists>
constexpr const DestinationPos GetDestinationPosV =
    GetDestinationPos<I, Lists...>::value;

template <typename... Lists>
struct CatLists;

template <typename... CurrTypes, typename... NextTypes, typename... NextLists>
struct CatLists<TypeList<CurrTypes...>, TypeList<NextTypes...>, NextLists...> {
  using type =
      CatLists<TypeList<CurrTypes..., NextTypes...>, NextLists...>::type;
};

template <typename... CurrTypes>
struct CatLists<TypeList<CurrTypes...>> {
  using type = TypeList<CurrTypes...>;
};

template <typename... Lists>
using CatListsT = CatLists<Lists...>::type;

namespace details {
template <typename TupleT>
struct TupleAsTypeListImpl;

template <typename... TupleTypesT>
struct TupleAsTypeListImpl<kxi::flat_tuple::Tuple<TupleTypesT...>> {
  using type = TypeList<TupleTypesT...>;
};
}  // namespace details

template <typename TupleT>
struct TupleAsTypeList {
  using type = details::TupleAsTypeListImpl<std::remove_cvref_t<TupleT>>::type;
};

template <typename TupleT>
using TupleAsTypeListT = TupleAsTypeList<TupleT>::type;

template <typename TypeListT>
struct TypeListAsTuple;

template <typename... TypeListTypesT>
struct TypeListAsTuple<TypeList<TypeListTypesT...>> {
  using type = kxi::flat_tuple::Tuple<TypeListTypesT...>;
};

template <typename TypeListT>
using TypeListAsTupleT = TypeListAsTuple<TypeListT>::type;

};  // namespace kxi::type_list