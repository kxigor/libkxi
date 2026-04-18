#pragma once

#include <cstddef>

#include "concepts.hpp"
#include "ops_details.hpp"

namespace kxi::tuple::like {
template <std::size_t I, concepts::TupleLike TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<TupleT>(tuple).template Get<I>();
}

template <typename T, concepts::DistinctTupleLike TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward<TupleT>(tuple).template Get<T>();
}

template <concepts::SwappableTuple TupleT>
constexpr void Swap(TupleT& lhs, TupleT& rhs) noexcept(
    noexcept(std::declval<TupleT&>().Swap(std::declval<TupleT&>()))) {
  lhs.Swap(rhs);
}

template <std::size_t I, concepts::TupleLike... TuplesT>
constexpr decltype(auto) PackGet(TuplesT&&... tuples) {
  using FlatGetHelperT = details::PackGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;

  RefsTupleT tuple_of_tuples{tuples...};
  return FlatGetHelperT::template Get<I>(tuple_of_tuples);
}

}  // namespace kxi::tuple::like