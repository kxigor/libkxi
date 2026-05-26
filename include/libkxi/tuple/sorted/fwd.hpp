#pragma once

namespace kxi::tuple::sorted {

template <template <typename LHS, typename RHS> typename Pred>
struct Of {
  template <typename... Args>
  class Tuple;
};

}  // namespace kxi::tuple::sorted
