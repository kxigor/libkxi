#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>
#include <libkxi/tuple/flat.hpp>
#include <libkxi/utility.hpp>
#include <type_traits>
#include <utility>

#include "fwd.hpp"

namespace kxi::tuple::sorted {

template <template <typename LHS, typename RHS> typename Pred>
template <typename... Types>
class Of<Pred>::Tuple {
 private:
  /*========================== Usings ==========================*/
  using SorterT = meta::Sort<Pred, flat::Tuple<Types...>>;
  using BaseTupleT = SorterT::SortedShellT;

 public:
  /*================= Constructors/Destructors =================*/
  constexpr Tuple() = default;

  constexpr Tuple(const Tuple& /*unused*/) = default;

  constexpr Tuple(Tuple&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<Tuple, Args...> &&
           (sizeof...(Args) == sizeof...(Types))
  constexpr Tuple(Args&&... args)
      : tuple_(
            /*copy elision works here*/
            [&]<std::size_t... Is>(std::index_sequence<Is...> /*unused*/) {
              using ArgsTypeListT = meta::pack::PackHolder<Args...>;
              using SavedArgsTupleT =
                  flat::Tuple<std::remove_reference_t<Args>&...>;

              SavedArgsTupleT tuple_args{args...};

              return BaseTupleT{std::forward<
                  meta::TypeAtT<SorterT::kPermutation[Is], ArgsTypeListT>>(
                  tuple_args.template Get<SorterT::kPermutation[Is]>())...};
            }(std::make_index_sequence<sizeof...(Args)>{})) {}

  constexpr ~Tuple() = default;

  /*======================= Assignments ========================*/
  constexpr Tuple& operator=(const Tuple& /*unused*/) = default;

  constexpr Tuple& operator=(Tuple&& /*unused*/) = default;

  /*======================== Interface =========================*/
  template <std::size_t I, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward_like<Self>(self.tuple_)
        .template Get<SorterT::kInversePermutation[I]>();
  }

  template <typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward_like<Self>(self.tuple_).template Get<T>();
  }

  constexpr void Swap(Tuple& other) noexcept(
      noexcept(std::declval<BaseTupleT&>().Swap(std::declval<BaseTupleT&>()))) {
    tuple_.Swap(other.tuple_);
  }

 private:
  /*======================= Data fields ========================*/
  BaseTupleT tuple_;
};

};  // namespace kxi::tuple::sorted