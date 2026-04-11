#pragma once

#include <libkxi/heterogeneous.hpp>
#include <libkxi/tuple/fwd.hpp>

namespace kxi::tuple {

template <typename T>
struct IsTuple : het::IsHeterogeneous<FlatTuple, T> {};

template <typename T>
constexpr const bool IsTupleV = IsTuple<T>::value;

namespace concepts {
template <typename T>
concept FlatTuple = IsTupleV<T>;
}  // namespace concepts

}  // namespace kxi::tuple
