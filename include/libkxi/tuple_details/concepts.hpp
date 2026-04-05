#pragma once

#include <libkxi/tuple_details/fwd.hpp>
#include <libkxi/utility.hpp>

namespace kxi::tuple {

template <typename T>
struct IsTupleSpec : utility::IsTypeSpecOf<T, Tuple> {};

template <typename T>
constexpr const bool IsTupleSpecV = IsTupleSpec<T>::value;

namespace concepts {
template <typename T>
concept Tuple = IsTupleSpecV<T>;
}  // namespace concepts

}  // namespace kxi::tuple
