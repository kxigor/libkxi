#pragma once

#include <cstddef>
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

template <std::size_t I, typename T>
struct IndexedType {
 public:
  /*===================== Usings/Constants =====================*/
  static constexpr std::size_t kHolderNumber = I;
  using ValueT = T;

  /*================= Constructors/Destructors =================*/
  constexpr IndexedType() = default;

  constexpr IndexedType(const IndexedType& /*unused*/) = default;

  constexpr IndexedType(IndexedType&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<IndexedType, Args...> &&
           std::is_constructible_v<ValueT, Args...>
  constexpr IndexedType(Args&&... args) : value_(std::forward<Args>(args)...) {}

  constexpr ~IndexedType() = default;

  /*======================= Assignments ========================*/
  constexpr IndexedType& operator=(const IndexedType& /*unused*/) = default;

  constexpr IndexedType& operator=(IndexedType&& /*unused*/) = default;

  /*========================= Getters ==========================*/
  ValueT& Value() { return value_; }

  const ValueT& Value() const { return value_; }

  volatile ValueT& Value() volatile { return value_; }

  const volatile ValueT& Value() const volatile { return value_; }

 private:
  /*======================= Data fields ========================*/
  ValueT value_{};
};

}  // namespace kxi::utility