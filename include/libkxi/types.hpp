#pragma once

#include <libkxi/heterogeneous.hpp>

namespace kxi::het {
template <typename... Args>
struct Types {};

template <typename... Args>
struct IsTypes : IsHeterogeneous<Types, Args...> {};

template <typename... Args>
inline constexpr const auto IsTypesV = IsTypes<Args...>::value;

namespace concepts {
template <typename... Args>
concept Types = IsTypesV<Args...>;
}

}  // namespace kxi::het