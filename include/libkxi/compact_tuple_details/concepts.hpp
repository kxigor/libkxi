#pragma once

#include <libkxi/compact_tuple_details/core.hpp>
#include <libkxi/utility.hpp>

namespace kxi::compact_tuple {

template <typename T>
struct IsCompactTupleSpec : utility::IsTypeSpecOf<T, CompactTuple> {};

template <typename T>
constexpr const bool IsCompactTupleSpecV = IsCompactTupleSpec<T>::value;

namespace concepts {
template <typename T>
concept CompactTuple = IsCompactTupleSpecV<T>;
}  // namespace concepts

};  // namespace kxi::compact_tuple