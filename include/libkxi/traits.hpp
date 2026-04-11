#pragma once

#include <cstddef>

namespace kxi::traits {
template <std::size_t Value>
struct SizeConstant {
  static constexpr const std::size_t value = Value;
};
}  // namespace kxi::traits