#pragma once

#include <libkxi/utility.hpp>

namespace kxi::het {

namespace details {
template <template <typename...> typename Container, typename T>
struct IsHeterogeneousImpl : std::false_type {};

template <template <typename...> typename Container, typename... Args>
struct IsHeterogeneousImpl<Container, Container<Args...>> : std::true_type {};
}  // namespace details

template <template <typename...> typename Container, typename T>
struct IsHeterogeneous
    : details::IsHeterogeneousImpl<Container, std::remove_cvref_t<T>> {};

template <template <typename...> typename Container, typename T>
inline constexpr const bool IsHeterogeneousV =
    IsHeterogeneous<Container, T>::value;

namespace concepts {
template <template <typename...> typename Container, typename T>
concept Heterogeneous = IsHeterogeneousV<Container, T>;
}

};  // namespace kxi::het
