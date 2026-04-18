#pragma once

#include <libkxi/tuple/sorted.hpp>

namespace kxi::tuple {

/*TODO: impl*/
template <typename LHS, typename RHS>
struct CompactPredicat;

template <typename... Args>
using CompactTuple = tuple::SortedTuple<CompactPredicat, Args...>;

};  // namespace kxi::tuple