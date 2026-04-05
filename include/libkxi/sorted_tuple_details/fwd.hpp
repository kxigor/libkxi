#pragma once

namespace kxi::sorted_tuple {
template <template <typename LHS, typename RHS> typename Predicat,
          typename... Args>
class SortedTuple;
}  // namespace kxi::sorted_tuple
