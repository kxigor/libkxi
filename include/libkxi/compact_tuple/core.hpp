#pragma once

#include <libkxi/sorted_tuple.hpp>

namespace kxi::tuple {

/*TODO: impl*/
template <typename LHS, typename RHS>
struct CompactPredicat;

template <typename... Args>
using CompactTuple = tuple::SortedTuple<CompactPredicat, Args...>;

};  // namespace kxi::tuple