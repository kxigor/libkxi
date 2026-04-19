#pragma once

#include <libkxi/tuple/like.hpp>

#include "core.hpp"

namespace kxi::tuple::compact {
using like::Cat;
using like::Get;
using like::SpanGet;
using like::Swap;

template <typename... Args>
constexpr auto Forward(Args&&... args) {
  return like::Forward<CompactTuple>(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto ForwardInto(Args&&... args) {
  return like::ForwardInto<CompactTuple>(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto Tie(Args&... args) {
  return like::Tie<CompactTuple>(args...);
}

template <typename... Args>
constexpr auto Make(Args&&... args) {
  return like::Make<CompactTuple>(std::forward<Args>(args)...);
}
}  // namespace kxi::tuple::compact