#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>
#include <libkxi/tuple/core.hpp>
#include <libkxi/tuple/fwd.hpp>
#include <libkxi/tuple/ops_details.hpp>
#include <libkxi/types.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple {

template <std::size_t I, concepts::FlatTuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<TupleT>(tuple).template Get<I>();
}

template <typename T, concepts::FlatTuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<TupleT>(tuple).template Get<T>();
}

template <concepts::FlatTuple TupleT>
constexpr void Swap(TupleT& lhs, TupleT& rhs) noexcept(
    noexcept(std::declval<TupleT&>().Swap(std::declval<TupleT&>()))) {
  lhs.Swap(rhs);
}

template <std::size_t I, concepts::FlatTuple... TuplesT>
constexpr decltype(auto) FlatGet(TuplesT&&... tuples) {
  using FlatGetHelperT = details::FlatGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;

  RefsTupleT tuple_of_tuples{tuples...};
  return FlatGetHelperT::template Get<I>(tuple_of_tuples);
}

template <typename... Args>
constexpr auto Tie(Args&... args) {
  using ResultType = FlatTuple<Args&...>;
  return ResultType{args...};
}

template <typename... Args>
constexpr auto MakeTuple(Args&&... args) {
  using ResultType = FlatTuple<std::unwrap_ref_decay_t<Args>...>;
  return ResultType{std::forward<Args>(args)...};
}

template <concepts::FlatTuple... TuplesT>
constexpr auto TupleCat(TuplesT&&... tuples) {
  using FlatGetHelperT = details::FlatGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;
  using ResultType = FlatGetHelperT::MergedPackT;

  RefsTupleT tuple_of_tuples{tuples...};

  return
      [&]<std::size_t... Indexes>(std::index_sequence<Indexes...> /*unused*/) {
        return ResultType{
            FlatGetHelperT::template Get<Indexes>(tuple_of_tuples)...};
      }(std::make_index_sequence<FlatGetHelperT::kTotalSize>{});
}

template <typename... Args>
constexpr auto ForwardAsTuple(Args&&... args) {
  return FlatTuple<Args&&...>{std::forward<Args>(args)...};
}

}  // namespace kxi::tuple