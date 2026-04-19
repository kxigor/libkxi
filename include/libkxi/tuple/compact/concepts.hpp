#pragma once

#include <libkxi/meta.hpp>

#include "core.hpp"

namespace kxi::tuple::compact {

template <typename T>
struct IsCompactTuple : meta::IsHeterogeneous<CompactTuple, T> {};

template <typename T>
constexpr const bool IsCompactTupleV = IsCompactTuple<T>::value;

namespace concepts {
template <typename T>
concept CompactTuple = IsCompactTupleV<T>;
}  // namespace concepts

};  // namespace kxi::tuple::compact