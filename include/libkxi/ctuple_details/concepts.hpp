#pragma once

#include <libkxi/ctuple_details/fwd.hpp>
#include <libkxi/utility.hpp>

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