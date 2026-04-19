#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>  // IWYU pragma: keep
#include <libkxi/utility.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple::flat::details {

template <utility::concepts::IndexSequence IndexesCortage,
          meta::pack::concepts::PackHolder TypesCortage>
struct TupleImpl;

template <std::size_t... Indexes, typename... Types>
class TupleImpl<std::index_sequence<Indexes...>,
                meta::pack::PackHolder<Types...>>
    : utility::IndexedType<Indexes, Types>... {
  /*====================== Usings/Helpers ======================*/
 private:
  using IndexesCortage = std::index_sequence<Indexes...>;
  using TypesCortage = meta::pack::PackHolder<Types...>;

  /*================= Constructors/Destructors =================*/
 public:
  constexpr TupleImpl() = default;

  constexpr TupleImpl(const TupleImpl& /*unused*/) = default;

  constexpr TupleImpl(TupleImpl&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<TupleImpl, Args...>
  constexpr TupleImpl(Args&&... args)
      : utility::IndexedType<Indexes, Types>(std::forward<Args>(args))... {}

  constexpr ~TupleImpl() = default;

  /*======================= Assignments ========================*/
  constexpr TupleImpl& operator=(const TupleImpl& /*unused*/) = default;

  constexpr TupleImpl& operator=(TupleImpl&& /*unused*/) = default;

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

  constexpr void Swap(TupleImpl& other) noexcept(
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