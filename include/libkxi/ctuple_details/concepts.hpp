#pragma once

#include "../utility.hpp"
#include "ctuple_fwd.hpp"

namespace kxi::ctuple {

template <typename T>
struct IsCtupleSpec : utility::IsSpecOf<T, Ctuple> {};

template <typename T>
constexpr const bool IsCtupleSpecV = IsCtupleSpec<T>::value;

namespace concepts {
template <typename T>
concept Ctuple = IsCtupleSpecV<T>;
}  // namespace concepts

};  // namespace kxi::ctuple