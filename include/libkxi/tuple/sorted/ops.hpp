#pragma once

#include <libkxi/tuple/like.hpp>

#include "fwd.hpp"

namespace kxi::tuple::sorted {
using like::Cat;
using like::Get;
using like::SpanGet;
using like::Swap;

template <typename... Args>
constexpr auto Forward(Args&&... args) {
  return like::Forward<SortedTuple>(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto ForwardInto(Args&&... args) {
  return like::ForwardInto<SortedTuple>(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto Tie(Args&... args) {
  return like::Tie<SortedTuple>(args...);
}

template <typename... Args>
constexpr auto Make(Args&&... args) {
  return like::Make<SortedTuple>(std::forward<Args>(args)...);
}
}  // namespace kxi::tuple::sorted