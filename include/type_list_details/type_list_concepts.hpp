#pragma once

#include "../utility.hpp"
#include "type_list_fwd.hpp"

namespace kxi::type_list {

template <typename T>
struct IsTypeListSpec : utility::IsSpecOf<T, TypeList> {};

template <typename T>
constexpr const bool IsTypeListSpecV = IsTypeListSpec<T>::value;

namespace concepts {
template <typename T>
concept TypeList = IsTypeListSpecV<T>;
}

}  // namespace kxi::type_list