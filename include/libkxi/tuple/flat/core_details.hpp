#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>  // IWYU pragma: keep
#include <libkxi/types.hpp>
#include <libkxi/utility.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple::flat::details {

template <utility::concepts::IndexSequence IndexesCortage,
          meta::concepts::Types TypesCortage>

struct FlatTupleImpl;

template <std::size_t... Indexes, typename... Types>
class FlatTupleImpl<std::index_sequence<Indexes...>, meta::Types<Types...>>
    : utility::IndexedType<Indexes, Types>... {
  /*====================== Usings/Helpers ======================*/
 private:
  using IndexesCortage = std::index_sequence<Indexes...>;
  using TypesCortage = meta::Types<Types...>;

  /*================= Constructors/Destructors =================*/
 public:
  constexpr FlatTupleImpl() = default;

  constexpr FlatTupleImpl(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl(FlatTupleImpl&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<FlatTupleImpl, Args...>
  constexpr FlatTupleImpl(Args&&... args)
      : utility::IndexedType<Indexes, Types>(std::forward<Args>(args))... {}

  constexpr ~FlatTupleImpl() = default;

  /*======================= Assignments ========================*/
  constexpr FlatTupleImpl& operator=(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl& operator=(FlatTupleImpl&& /*unused*/) = default;

  /*======================== Interface =========================*/
  template <std::size_t I, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<I, meta::TypeAtT<I, TypesCortage>>();
  }

  template <typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<meta::IndexOfV<T, TypesCortage>>();
  }

  constexpr void Swap(FlatTupleImpl& other) noexcept(
      (std::is_nothrow_swappable_v<Types> && ...)) {
    (std::swap(Get<Indexes>(), other.Get<Indexes>()), ...);
  }

  /*========================== Impls ===========================*/
 private:
  template <std::size_t I, typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    using BaseT = kxi::utility::CopyCVT<Self, utility::IndexedType<I, T>>;
    return std::forward_like<Self>(static_cast<BaseT&>(self).Value());
  }
};

}  // namespace kxi::tuple::flat::details