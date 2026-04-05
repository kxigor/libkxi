#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include "../type_list.hpp"
#include "tuple_core.hpp"
#include "tuple_fwd.hpp"
#include "tuple_ops_details.hpp"

namespace kxi::flat_tuple {

template <std::size_t I, typename T, concepts::Tuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template Get<I, T>();
}

template <std::size_t I, concepts::Tuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template Get<I>();
}

template <typename T, concepts::Tuple TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template Get<T>();
}

template <std::size_t I, concepts::Tuple... TuplesT>
constexpr decltype(auto) FlatGet(TuplesT&&... tuples) {
  using FlatGetImplT = details::FlatGetImpl<TuplesT...>;
  using TupleOfTuplesT = FlatGetImplT::TupleOfTuplesT;

  TupleOfTuplesT tuple_of_tuples{tuples...};
  return FlatGetImplT::template Get<I>(tuple_of_tuples);
}

template <typename... Args>
constexpr auto Tie(Args&... args) {
  using ResultType = Tuple<Args&...>;
  return ResultType{args...};
}

template <typename... Args>
constexpr auto MakeTuple(Args&&... args) {
  using ResultType = Tuple<std::unwrap_ref_decay_t<Args>...>;
  return ResultType{std::forward<Args>(args)...};
}

template <concepts::Tuple... TuplesT>
constexpr auto TupleCat(TuplesT&&... tuples) {
  using FlatGetImplT = details::FlatGetImpl<TuplesT...>;
  using TupleOfTuplesT = FlatGetImplT::TupleOfTuplesT;
  using ResultType =
      type_list::TypeListAsTupleT<typename FlatGetImplT::TypeListOfAllTypes>;

  TupleOfTuplesT tuple_of_tuples{tuples...};

  return [&]<std::size_t... Indexes>(
             std::index_sequence<Indexes...> /*unused*/) {
    return ResultType{FlatGetImplT::template Get<Indexes>(tuple_of_tuples)...};
  }(std::make_index_sequence<FlatGetImplT::kNumberOfAllTypes>{});
}

}  // namespace kxi::flat_tuple