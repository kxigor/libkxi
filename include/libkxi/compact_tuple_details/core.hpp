#pragma once

#include <libkxi/sort_tuple.hpp>

namespace kxi::compact_tuple {

/*TODO: impl*/
struct CompactPredicat;

template <typename... Args>
using CompactTuple = sorted_tuple::SortedTuple<CompactPredicat, Args...>;

};  // namespace kxi::compact_tuple