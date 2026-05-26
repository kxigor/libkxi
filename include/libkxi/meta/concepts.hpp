#pragma once

#include <type_traits>

namespace kxi::meta {

namespace details {
template <typename...>
struct PackTester {};

template <typename, typename = void>
struct IsTListImpl : std::false_type {};

template <template <typename...> typename Shell, typename... Args>
struct IsTListImpl<Shell<Args...>,
                   std::void_t<Shell<PackTester<>, PackTester<Args>...>>>
    : std::true_type {};
}  // namespace details

template <typename T>
struct IsTList : details::IsTListImpl<std::remove_cvref_t<T>> {};

template <typename T>
inline constexpr bool IsTListV = IsTList<T>::value;

namespace concepts {
template <typename T>
concept TList = IsTListV<T>;
}  // namespace concepts

};  // namespace kxi::meta
