#pragma once

#include <cstddef>
#include <libkxi/heterogeneous.hpp>
#include <libkxi/tuple/core.hpp>
#include <libkxi/tuple/fwd.hpp>
#include <libkxi/tuple/ops_details.hpp>
#include <libkxi/types.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple {

template <std::size_t I, typename T, concepts::FlatTuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<TupleT>(tuple).template Get<I, T>();
}

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
  using TupleOfTuplesT = FlatGetHelperT::TupleOfTuplesT;

  TupleOfTuplesT tuple_of_tuples{tuples...};
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
  using TupleOfTuplesT = FlatGetHelperT::TupleOfTuplesT;
  using ResultType = FlatGetHelperT::TypeListOfAllTypes;

  TupleOfTuplesT tuple_of_tuples{tuples...};

  return
      [&]<std::size_t... Indexes>(std::index_sequence<Indexes...> /*unused*/) {
        return ResultType{
            FlatGetHelperT::template Get<Indexes>(tuple_of_tuples)...};
      }(std::make_index_sequence<FlatGetHelperT::kNumberOfAllTypes>{});
}

template <typename... Args>
constexpr auto ForwardAsTuple(Args&&... args) {
  return FlatTuple<Args&&...>{std::forward<Args>(args)...};
}

}  // namespace kxi::tuple