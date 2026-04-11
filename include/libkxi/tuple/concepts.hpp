#pragma once

#include <libkxi/meta.hpp>
#include <libkxi/tuple/fwd.hpp>

namespace kxi::tuple {

template <typename T>
struct IsFlatTuple : meta::IsHeterogeneous<FlatTuple, T> {};

template <typename T>
constexpr const bool IsFlatTupleV = IsFlatTuple<T>::value;

namespace concepts {
template <typename T>
concept FlatTuple = IsFlatTupleV<T>;
}  // namespace concepts

}  // namespace kxi::tuple
