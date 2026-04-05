#pragma once

#include <libkxi/sorted_tuple_details/fwd.hpp>
#include <libkxi/utility.hpp>

namespace kxi::sorted_tuple {

namespace details {
template <typename T>
struct IsSortedTupleSpecImpl {
  static constexpr const bool value = false;
};
template <template <typename LHS, typename RHS> typename Predicat,
          typename... Args>
struct IsSortedTupleSpecImpl<SortedTuple<Predicat, Args...>> {
  static constexpr const bool value = true;
};
}  // namespace details

template <typename T>
struct IsSortedTupleSpec
    : details::IsSortedTupleSpecImpl<std::remove_cvref_t<T>> {};

template <typename T>
constexpr const bool IsSortedTupleSpecV = IsSortedTupleSpec<T>::value;

namespace concepts {
template <typename T>
concept SortedTuple = IsSortedTupleSpecV<T>;
}  // namespace concepts

};  // namespace kxi::sorted_tuple