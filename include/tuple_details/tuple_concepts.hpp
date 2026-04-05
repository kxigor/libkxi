#pragma once

#include "../utility.hpp"
#include "tuple_fwd.hpp"

namespace kxi::tuple {

template <typename T>
struct IsTupleSpec : utility::IsSpecOf<T, Tuple> {};

template <typename T>
constexpr const bool IsTupleSpecV = IsTupleSpec<T>::value;

namespace concepts {
template <typename T>
concept Tuple = IsTupleSpecV<T>;
}  // namespace concepts

}  // namespace kxi::tuple
