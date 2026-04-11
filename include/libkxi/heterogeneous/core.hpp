#pragma once

#include <array>
#include <cstddef>
#include <libkxi/heterogeneous/concepts.hpp>
#include <libkxi/traits.hpp>
#include <type_traits>

namespace kxi::het {

namespace details {
template <template <typename...> typename Container, typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
struct GetSizeImpl;

template <template <typename...> typename Container, typename... Types>
struct GetSizeImpl<Container, Container<Types...>>
    : traits::SizeConstant<sizeof...(Types)> {};
}  // namespace details

template <template <typename...> typename Container, typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
using GetSize =
    details::GetSizeImpl<Container, std::remove_cvref_t<Heterogeneous>>;

template <template <typename...> typename Container, typename Heterogeneous>
inline constexpr const std::size_t GetSizeV =
    GetSize<Container, Heterogeneous>::value;

namespace details {
template <template <typename...> typename Container, std::size_t I,
          typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
struct GetTypeImpl;

template <template <typename...> typename Container, std::size_t I,
          typename Head, typename... Tail>
struct GetTypeImpl<Container, I, Container<Head, Tail...>> {
  using type = GetTypeImpl<Container, I - 1, Container<Tail...>>::type;
};

template <template <typename...> typename Container, typename Head,
          typename... Tail>
struct GetTypeImpl<Container, 0, Container<Head, Tail...>> {
  using type = Head;
};
}  // namespace details

template <template <typename...> typename Container, std::size_t I,
          typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
using GetType =
    details::GetTypeImpl<Container, I, std::remove_cvref_t<Heterogeneous>>;

template <template <typename...> typename Container, std::size_t I,
          typename Heterogeneous>
using GetTypeT = typename GetType<Container, I, Heterogeneous>::type;

namespace details {
template <template <typename...> typename Container, typename T,
          typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
struct GetCountImpl;

template <template <typename...> typename Container, typename T, typename Head,
          typename... Tail>
struct GetCountImpl<Container, T, Container<Head, Tail...>>
    : traits::SizeConstant<
          GetCountImpl<Container, T, Container<Tail...>>::value> {};

template <template <typename...> typename Container, typename Head,
          typename... Tail>
struct GetCountImpl<Container, Head, Container<Head, Tail...>>
    : traits::SizeConstant<
          1 + GetCountImpl<Container, Head, Container<Tail...>>::value> {};

template <template <typename...> typename Container, typename Head>
struct GetCountImpl<Container, Head, Container<>> : traits::SizeConstant<0> {};
}  // namespace details

template <template <typename...> typename Container, typename T,
          typename Heterogeneous>
requires concepts::Heterogeneous<Container, Heterogeneous>
using GetCount =
    details::GetCountImpl<Container, T, std::remove_cvref_t<Heterogeneous>>;

namespace details {
template <template <typename...> typename Container, typename T,
          typename Heterogeneous>
requires(GetCountImpl<Container, T, Heterogeneous>::value == 1)
struct GetIndexImpl;

template <template <typename...> typename Container, typename T, typename Head,
          typename... Tail>
struct GetIndexImpl<Container, T, Container<Head, Tail...>>
    : traits::SizeConstant<
          1 + GetIndexImpl<Container, T, Container<Tail...>>::value> {};

template <template <typename...> typename Container, typename Head,
          typename... Tail>
struct GetIndexImpl<Container, Head, Container<Head, Tail...>>
    : traits::SizeConstant<0> {};
}  // namespace details

template <template <typename...> typename Container, typename T,
          typename Heterogeneous>
requires(GetCount<Container, T, Heterogeneous>::value == 1)
using GetIndex =
    details::GetIndexImpl<Container, T, std::remove_cvref_t<Heterogeneous>>;

template <template <typename...> typename Container, typename T,
          typename Heterogeneous>
inline constexpr const std::size_t GetIndexV =
    GetIndex<Container, T, Heterogeneous>::value;

namespace details {
template <template <typename...> typename Container, std::size_t I,
          typename... Heterogeneous>
struct GetDestinationPosImpl {
 public:
  struct DestinationPos {
    std::size_t list_pos{};
    std::size_t elem_pos{};
  };

 private:
  static constexpr DestinationPos Get() {
    static constexpr std::array kListSizes = {
        GetSizeImpl<Container, Heterogeneous>::value...};
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

template <template <typename...> typename Container, std::size_t I,
          typename... Heterogeneous>
using GetDestinationPos =
    details::GetDestinationPosImpl<Container, I,
                                   std::remove_cvref_t<Heterogeneous>...>;

template <template <typename...> typename Container, std::size_t I,
          typename... Heterogeneous>
inline constexpr const auto GetDestinationPosV =
    GetDestinationPos<Container, I, Heterogeneous...>::value;

namespace details {
template <template <typename...> typename Container, typename... Heterogeneous>
struct CatListsImpl;

template <template <typename...> typename Container, typename... CurrTypes,
          typename... NextTypes, typename... NextHeterogeneous>
struct CatListsImpl<Container, Container<CurrTypes...>, Container<NextTypes...>,
                    NextHeterogeneous...> {
  using type = CatListsImpl<Container, Container<CurrTypes..., NextTypes...>,
                            NextHeterogeneous...>::type;
};

template <template <typename...> typename Container, typename... CurrTypes>
struct CatListsImpl<Container, Container<CurrTypes...>> {
  using type = Container<CurrTypes...>;
};
}  // namespace details

template <template <typename...> typename Container, typename... Heterogeneous>
using CatLists =
    details::CatListsImpl<Container, std::remove_cvref_t<Heterogeneous>...>;

template <template <typename...> typename Container, typename... Heterogeneous>
using CatListsT = CatLists<Container, Heterogeneous...>::type;

};  // namespace kxi::het