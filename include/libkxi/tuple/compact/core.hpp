#pragma once

#include <libkxi/tuple/sorted.hpp>
#include <type_traits>

namespace kxi::tuple::compact {

template <typename LHS, typename RHS>
struct CompactPredicat
    : std::integral_constant<bool, (alignof(LHS) > alignof(RHS))> {};

template <typename... Args>
using CompactTuple = sorted::Of<CompactPredicat>::Tuple<Args...>;

};  // namespace kxi::tuple::compact