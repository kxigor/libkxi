#pragma once

#include <cstddef>

namespace kxi::traits {
template <typename Type, Type Value>
struct IntegralConstant {
  using type = Type;
  static constexpr Type value = Value;
};

template <bool Value>
using BoolConstant = IntegralConstant<bool, Value>;

using FalseType = BoolConstant<false>;
using TrueType = BoolConstant<true>;

template <std::size_t Value>
using SizeConstant = IntegralConstant<std::size_t, Value>;

}  // namespace kxi::traits