#pragma once

#include <libkxi/meta.hpp>

namespace kxi::meta {
template <typename... Args>
struct Types {};

template <typename... Args>
struct IsTypes : IsHeterogeneous<Types, Args...> {};

template <typename... Args>
inline constexpr const auto IsTypesV = IsTypes<Args...>::value;

namespace concepts {
template <typename... Args>
concept Types = IsTypesV<Args...>;
}  // namespace concepts

}  // namespace kxi::meta