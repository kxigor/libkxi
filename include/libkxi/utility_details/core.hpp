#pragma once

#include <cstddef>
#include <libkxi/utility_details/core_details.hpp>
#include <type_traits>
#include <utility>

namespace kxi::utility {
template <typename From, typename To>
struct CopyConst {
  using type = To;
};

template <typename From, typename To>
struct CopyConst<const From, To> {
  using type = const To;
};

template <typename From, typename To>
using CopyConstT = typename CopyConst<From, To>::type;

namespace details {

template <typename From>
struct CopyCVImpl {
  template <typename To>
  using type = To;
};

template <typename From>
struct CopyCVImpl<const From> {
  template <typename To>
  using type = const To;
};

template <typename From>
struct CopyCVImpl<volatile From> {
  template <typename To>
  using type = volatile To;
};

template <typename From>
struct CopyCVImpl<const volatile From> {
  template <typename To>
  using type = const volatile To;
};

}  // namespace details

template <typename From, typename To>
using CopyCVT = typename details::CopyCVImpl<
    std::remove_reference_t<From>>::template type<std::remove_reference_t<To>>;

template <typename T, template <typename...> typename Template>
struct IsTypeSpecOf {
  static constexpr const bool value =
      details::IsTypeSpecOfImpl<std::remove_cvref_t<T>, Template>::value;
};

template <typename T, template <typename...> typename Template>
constexpr const bool IsTypeSpecOfV = IsTypeSpecOf<T, Template>::value;

template <typename T, template <auto...> typename Template>
struct IsValueSpecOf {
  static constexpr const bool value =
      details::IsValueSpecOfImpl<T, Template>::value;
};

template <typename T, template <auto...> typename Template>
constexpr const bool IsValueSpecOfV = IsValueSpecOf<T, Template>::value;

template <typename T>
struct IsIndexSequence : std::false_type {
  static constexpr const bool value = false;
};

template <std::size_t... Is>
struct IsIndexSequence<std::index_sequence<Is...>> {
  static constexpr const bool value = true;
};

template <typename T>
constexpr const bool IsIndexSequenceV = IsIndexSequence<T>::value;

namespace concepts {
template <typename Self, typename... Args>
concept PerfectCtorGuard =
    (sizeof...(Args) != 1) ||
    (!std::is_same_v<std::remove_cvref_t<Args>, Self> && ...);

template <typename T>
concept IndexSequence = IsIndexSequenceV<T>;

}  // namespace concepts

}  // namespace kxi::utility