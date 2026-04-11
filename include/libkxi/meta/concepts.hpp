#pragma once

#include <libkxi/utility.hpp>

namespace kxi::meta {

namespace details {
template <template <typename...> typename Shell, typename T>
struct IsHeterogeneousImpl : std::false_type {};

template <template <typename...> typename Shell, typename... Args>
struct IsHeterogeneousImpl<Shell, Shell<Args...>> : std::true_type {};
}  // namespace details

template <template <typename...> typename Shell, typename T>
struct IsHeterogeneous
    : details::IsHeterogeneousImpl<Shell, std::remove_cvref_t<T>> {};

template <template <typename...> typename Shell, typename T>
inline constexpr bool IsHeterogeneousV = IsHeterogeneous<Shell, T>::value;

namespace concepts {
template <template <typename...> typename Shell, typename T>
concept Heterogeneous = IsHeterogeneousV<Shell, T>;
}  // namespace concepts

};  // namespace kxi::meta
