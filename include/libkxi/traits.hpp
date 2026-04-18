#pragma once

#include <cstddef>
#include <type_traits>

namespace kxi::traits {
template <std::size_t Value>
using SizeConstant = std::integral_constant<std::size_t, Value>;
}  // namespace kxi::traits