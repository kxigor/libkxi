#pragma once

#include <libkxi/sorted_tuple_details/fwd.hpp>
#include <libkxi/utility.hpp>

namespace kxi::sorted_tuple {

template <typename T>
struct IsSortedTupleSpec : utility::IsTypeSpecOf<T, SortedTuple> {};

template <typename T>
constexpr const bool IsSortedTupleSpecV = IsSortedTupleSpec<T>::value;

namespace concepts {
template <typename T>
concept SortedTuple = IsSortedTupleSpecV<T>;
}  // namespace concepts

};  // namespace kxi::sorted_tuple