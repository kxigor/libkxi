#pragma once

#include <array>
#include <cstddef>
#include <libkxi/meta/concepts.hpp>
#include <libkxi/traits.hpp>
#include <type_traits>

namespace kxi::meta {

namespace details {
template <concepts::VariadicInstance TList>
struct GetSizeImpl;

template <template <typename...> typename Shell, typename... Types>
struct GetSizeImpl<Shell<Types...>> : traits::SizeConstant<sizeof...(Types)> {};
}  // namespace details

template <concepts::VariadicInstance TList>
using GetSize = details::GetSizeImpl<std::remove_cvref_t<TList>>;

template <concepts::VariadicInstance TList>
inline constexpr std::size_t GetSizeV = GetSize<TList>::value;

namespace details {
template <std::size_t I, concepts::VariadicInstance TList>
struct GetTypeImpl;

template <template <typename...> typename Shell, std::size_t I, typename Head,
          typename... Tail>
struct GetTypeImpl<I, Shell<Head, Tail...>> {
  using type = GetTypeImpl<I - 1, Shell<Tail...>>::type;
};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetTypeImpl<0, Shell<Head, Tail...>> {
  using type = Head;
};
}  // namespace details

template <std::size_t I, concepts::VariadicInstance TList>
using GetType = details::GetTypeImpl<I, std::remove_cvref_t<TList>>;

template <std::size_t I, concepts::VariadicInstance TList>
using GetTypeT = typename GetType<I, TList>::type;

namespace details {
template <typename T, concepts::VariadicInstance TList>
struct GetCountImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct GetCountImpl<T, Shell<Head, Tail...>>
    : traits::SizeConstant<GetCountImpl<T, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetCountImpl<Head, Shell<Head, Tail...>>
    : traits::SizeConstant<1 + GetCountImpl<Head, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head>
struct GetCountImpl<Head, Shell<>> : traits::SizeConstant<0> {};
}  // namespace details

template <typename T, concepts::VariadicInstance TList>
using GetCount = details::GetCountImpl<T, std::remove_cvref_t<TList>>;

template <typename T, concepts::VariadicInstance TList>
inline constexpr std::size_t GetCountV = GetCount<T, TList>::value;

namespace details {
template <typename T, concepts::VariadicInstance TList>
struct GetIndexImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct GetIndexImpl<T, Shell<Head, Tail...>>
    : traits::SizeConstant<1 + GetIndexImpl<T, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetIndexImpl<Head, Shell<Head, Tail...>> : traits::SizeConstant<0> {};
}  // namespace details

template <typename T, concepts::VariadicInstance TList>
requires(GetCount<T, TList>::value == 1)
using GetIndex = details::GetIndexImpl<T, std::remove_cvref_t<TList>>;

template <typename T, concepts::VariadicInstance TList>
inline constexpr const std::size_t GetIndexV = GetIndex<T, TList>::value;

namespace details {
template <std::size_t I, concepts::VariadicInstance... TLists>
struct GetDestinationPosImpl {
 public:
  struct DestinationPos {
    std::size_t list_pos{};
    std::size_t elem_pos{};
  };

 private:
  static constexpr DestinationPos Get() {
    static constexpr std::array kListSizes = {GetSize<TLists>::value...};
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
  static constexpr const DestinationPos value = Get();
};
}  // namespace details

template <std::size_t I, concepts::VariadicInstance... TLists>
using GetDestinationPos =
    details::GetDestinationPosImpl<I, std::remove_cvref_t<TLists>...>;

template <std::size_t I, concepts::VariadicInstance... TLists>
inline constexpr auto GetDestinationPosV =
    GetDestinationPos<I, TLists...>::value;

namespace details {
template <concepts::VariadicInstance... TLists>
struct CatListsImpl;

template <template <typename...> typename Shell, typename... CurrTypes,
          typename... NextTypes, concepts::VariadicInstance... NextTLists>
struct CatListsImpl<Shell<CurrTypes...>, Shell<NextTypes...>, NextTLists...> {
  using type =
      CatListsImpl<Shell<CurrTypes..., NextTypes...>, NextTLists...>::type;
};

template <template <typename...> typename Shell, typename... CurrTypes>
struct CatListsImpl<Shell<CurrTypes...>> {
  using type = Shell<CurrTypes...>;
};
}  // namespace details

template <concepts::VariadicInstance... TLists>
using CatLists = details::CatListsImpl<std::remove_cvref_t<TLists>...>;

template <concepts::VariadicInstance... TLists>
using CatListsT = CatLists<TLists...>::type;

};  // namespace kxi::meta