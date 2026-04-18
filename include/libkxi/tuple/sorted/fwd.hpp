#pragma once

namespace kxi::tuple {
template <template <typename LHS, typename RHS> typename Predicat,
          typename... Args>
class SortedTuple;
}  // namespace kxi::tuple
