#pragma once

#include <libkxi/type_list_details/fwd.hpp>
#include <libkxi/utility.hpp>

namespace kxi::type_list {

template <typename T>
struct IsTypeListSpec : utility::IsTypeSpecOf<T, TypeList> {};

template <typename T>
constexpr const bool IsTypeListSpecV = IsTypeListSpec<T>::value;

namespace concepts {
template <typename T>
concept TypeList = IsTypeListSpecV<T>;
}

}  // namespace kxi::type_list