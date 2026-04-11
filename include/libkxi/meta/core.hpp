#pragma once

#include <array>
#include <cstddef>
#include <libkxi/meta/concepts.hpp>
#include <libkxi/traits.hpp>
#include <type_traits>

namespace kxi::meta {

namespace details {
template <template <typename...> typename Shell, typename TList>
struct GetSizeImpl;

template <template <typename...> typename Shell, typename... Types>
struct GetSizeImpl<Shell, Shell<Types...>>
    : traits::SizeConstant<sizeof...(Types)> {};
}  // namespace details

template <template <typename...> typename Shell, typename TList>
requires concepts::Heterogeneous<Shell, TList>
using GetSize = details::GetSizeImpl<Shell, std::remove_cvref_t<TList>>;

template <template <typename...> typename Shell, typename TList>
requires concepts::Heterogeneous<Shell, TList>
inline constexpr const std::size_t GetSizeV = GetSize<Shell, TList>::value;

namespace details {
template <template <typename...> typename Shell, std::size_t I, typename TList>
struct GetTypeImpl;

template <template <typename...> typename Shell, std::size_t I, typename Head,
          typename... Tail>
struct GetTypeImpl<Shell, I, Shell<Head, Tail...>> {
  using type = GetTypeImpl<Shell, I - 1, Shell<Tail...>>::type;
};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetTypeImpl<Shell, 0, Shell<Head, Tail...>> {
  using type = Head;
};
}  // namespace details

template <template <typename...> typename Shell, std::size_t I, typename TList>
requires concepts::Heterogeneous<Shell, TList>
using GetType = details::GetTypeImpl<Shell, I, std::remove_cvref_t<TList>>;

template <template <typename...> typename Shell, std::size_t I, typename TList>
requires concepts::Heterogeneous<Shell, TList>
using GetTypeT = typename GetType<Shell, I, TList>::type;

namespace details {
template <template <typename...> typename Shell, typename T, typename TList>
struct GetCountImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct GetCountImpl<Shell, T, Shell<Head, Tail...>>
    : traits::SizeConstant<GetCountImpl<Shell, T, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetCountImpl<Shell, Head, Shell<Head, Tail...>>
    : traits::SizeConstant<1 +
                           GetCountImpl<Shell, Head, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head>
struct GetCountImpl<Shell, Head, Shell<>> : traits::SizeConstant<0> {};
}  // namespace details

template <template <typename...> typename Shell, typename T, typename TList>
requires concepts::Heterogeneous<Shell, TList>
using GetCount = details::GetCountImpl<Shell, T, std::remove_cvref_t<TList>>;

template <template <typename...> typename Shell, typename T, typename TList>
requires concepts::Heterogeneous<Shell, TList>
inline constexpr std::size_t GetCountV = GetCount<Shell, T, TList>::value;

namespace details {
template <template <typename...> typename Shell, typename T, typename TList>
struct GetIndexImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct GetIndexImpl<Shell, T, Shell<Head, Tail...>>
    : traits::SizeConstant<1 + GetIndexImpl<Shell, T, Shell<Tail...>>::value> {
};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct GetIndexImpl<Shell, Head, Shell<Head, Tail...>>
    : traits::SizeConstant<0> {};
}  // namespace details

template <template <typename...> typename Shell, typename T, typename TList>
requires(concepts::Heterogeneous<Shell, TList> &&
         GetCount<Shell, T, TList>::value == 1)
using GetIndex = details::GetIndexImpl<Shell, T, std::remove_cvref_t<TList>>;

template <template <typename...> typename Shell, typename T, typename TList>
requires concepts::Heterogeneous<Shell, TList>
inline constexpr const std::size_t GetIndexV = GetIndex<Shell, T, TList>::value;

namespace details {
template <template <typename...> typename Shell, std::size_t I,
          typename... TLists>
struct GetDestinationPosImpl {
 public:
  struct DestinationPos {
    std::size_t list_pos{};
    std::size_t elem_pos{};
  };

 private:
  static constexpr DestinationPos Get() {
    static constexpr std::array kListSizes = {
        GetSizeImpl<Shell, TLists>::value...};
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

template <template <typename...> typename Shell, std::size_t I,
          typename... TLists>
requires(concepts::Heterogeneous<Shell, TLists> && ...)
using GetDestinationPos =
    details::GetDestinationPosImpl<Shell, I, std::remove_cvref_t<TLists>...>;

template <template <typename...> typename Shell, std::size_t I,
          typename... TLists>
requires(concepts::Heterogeneous<Shell, TLists> && ...)
inline constexpr auto GetDestinationPosV =
    GetDestinationPos<Shell, I, TLists...>::value;

namespace details {
template <template <typename...> typename Shell, typename... TLists>
struct CatListsImpl;

template <template <typename...> typename Shell, typename... CurrTypes,
          typename... NextTypes, typename... NextTLists>
struct CatListsImpl<Shell, Shell<CurrTypes...>, Shell<NextTypes...>,
                    NextTLists...> {
  using type = CatListsImpl<Shell, Shell<CurrTypes..., NextTypes...>,
                            NextTLists...>::type;
};

template <template <typename...> typename Shell, typename... CurrTypes>
struct CatListsImpl<Shell, Shell<CurrTypes...>> {
  using type = Shell<CurrTypes...>;
};
}  // namespace details

template <template <typename...> typename Shell, typename... TLists>
requires(concepts::Heterogeneous<Shell, TLists> && ...)
using CatLists = details::CatListsImpl<Shell, std::remove_cvref_t<TLists>...>;

template <template <typename...> typename Shell, typename... TLists>
requires(concepts::Heterogeneous<Shell, TLists> && ...)
using CatListsT = CatLists<Shell, TLists...>::type;

};  // namespace kxi::meta