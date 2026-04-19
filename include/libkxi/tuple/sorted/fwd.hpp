#pragma once

namespace kxi::tuple::sorted {
template <template <typename LHS, typename RHS> typename Predicat,
          typename... Args>
class SortedTuple;
}  // namespace kxi::tuple::sorted
