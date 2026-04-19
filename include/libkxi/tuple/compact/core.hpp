#pragma once

#include <libkxi/tuple/sorted.hpp>

namespace kxi::tuple::compact {

/*TODO: impl*/
template <typename LHS, typename RHS>
struct CompactPredicat;

template <typename... Args>
using CompactTuple = sorted::SortedTuple<CompactPredicat, Args...>;

};  // namespace kxi::tuple::compact