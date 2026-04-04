#pragma once

namespace kxi::flat_tuple {

template <typename... Args>
class Tuple;

namespace details {
template <typename T>
struct is_tuple_specialization_impl {
  static constexpr const bool value = false;
};

template <typename... TupleTypesT>
struct is_tuple_specialization_impl<Tuple<TupleTypesT...>> {
  static constexpr const bool value = true;
};
}  // namespace details

template <typename T>
struct is_tuple_specialization {
  static constexpr const bool value =
      details::is_tuple_specialization_impl<std::remove_cvref_t<T>>::value;
};

template <typename T>
constexpr const bool IsTupleSpecializationV = is_tuple_specialization<T>::value;

namespace concepts {
template <typename T>
concept Tuple = IsTupleSpecializationV<T>;
}

}  // namespace kxi::flat_tuple