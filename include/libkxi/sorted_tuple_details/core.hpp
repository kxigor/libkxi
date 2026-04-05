#pragma once

#include <libkxi/sorted_tuple_details/fwd.hpp>

namespace kxi::sorted_tuple {

template <template <typename LHS, typename RHS> typename Predicat,
          typename... Args>
class SortedTuple {};

};  // namespace kxi::sorted_tuple