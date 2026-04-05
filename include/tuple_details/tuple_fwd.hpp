#pragma once

#include <type_traits>

namespace kxi::flat_tuple {

template <typename... Args>
class Tuple;

namespace details {
template <typename T>
struct IsTupleSpecializationImpl {
  static constexpr const bool value = false;
};

template <typename... TupleTypesT>
struct IsTupleSpecializationImpl<Tuple<TupleTypesT...>> {
  static constexpr const bool value = true;
};
}  // namespace details

template <typename T>
struct IsTupleSpecialization {
  static constexpr const bool value =
      details::IsTupleSpecializationImpl<std::remove_cvref_t<T>>::value;
};

template <typename T>
constexpr const bool IsTupleSpecializationV = IsTupleSpecialization<T>::value;

namespace concepts {
template <typename T>
concept Tuple = IsTupleSpecializationV<T>;
}  // namespace concepts

}  // namespace kxi::flat_tuple