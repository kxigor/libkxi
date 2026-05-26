#pragma once

#include <libkxi/tuple/like.hpp>

#include "fwd.hpp"  // IWYU pragma: keep

namespace kxi::tuple::sorted {
using like::Cat;
using like::Get;
using like::SpanGet;
using like::Swap;

template <template <typename LHS, typename RHS> typename Pred, typename... Args>
constexpr auto Forward(Args&&... args) {
  return like::Forward<Of<Pred>::template Tuple>(std::forward<Args>(args)...);
}

template <template <typename LHS, typename RHS> typename Pred, typename... Args>
constexpr auto ForwardInto(Args&&... args) {
  return like::ForwardInto<Of<Pred>::template Tuple>(
      std::forward<Args>(args)...);
}

template <template <typename LHS, typename RHS> typename Pred, typename... Args>
constexpr auto Tie(Args&... args) {
  return like::Tie<Of<Pred>::template Tuple>(args...);
}

template <template <typename LHS, typename RHS> typename Pred, typename... Args>
constexpr auto Make(Args&&... args) {
  return like::Make<Of<Pred>::template Tuple>(std::forward<Args>(args)...);
}
}  // namespace kxi::tuple::sorted