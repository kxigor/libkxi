#pragma once

#include <type_traits>

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

namespace details {
template <typename...>
struct PackTester {};

template <typename, typename = void>
struct IsVariadicTemplateImpl : std::false_type {};

template <template <typename...> typename Shell, typename... Args>
struct IsVariadicTemplateImpl<
    Shell<Args...>, std::void_t<Shell<PackTester<>, PackTester<Args>...>>>
    : std::true_type {};
}  // namespace details

template <typename T>
struct IsVariadicTemplate
    : details::IsVariadicTemplateImpl<std::remove_cvref_t<T>> {};

template <typename T>
inline constexpr bool IsVariadicTemplateV = IsVariadicTemplate<T>::value;

namespace concepts {
template <typename T>
concept VariadicTemplate = IsVariadicTemplateV<T>;
}  // namespace concepts

};  // namespace kxi::meta
