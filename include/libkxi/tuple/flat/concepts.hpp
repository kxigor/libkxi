#pragma once

#include <libkxi/meta.hpp>

#include "fwd.hpp"

namespace kxi::tuple::flat {

template <typename T>
struct IsTuple : meta::IsHeterogeneous<Tuple, T> {};

template <typename T>
constexpr const bool IsTupleV = IsTuple<T>::value;

namespace concepts {
template <typename T>
concept Tuple = IsTupleV<T>;
}  // namespace concepts

}  // namespace kxi::tuple::flat
