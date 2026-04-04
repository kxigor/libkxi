#pragma once

#include "tuple_core.hpp"
#include "tuple_ops_details.hpp"

namespace kxi::flat_tuple {

template <std::size_t I, typename T, concepts::Tuple TupleT>
constexpr decltype(auto) get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template get<I, T>();
}

template <std::size_t I, concepts::Tuple TupleT>
constexpr decltype(auto) get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template get<I>();
}

template <typename T, concepts::Tuple TupleT>
constexpr decltype(auto) get(TupleT&& tuple) {
  return std::forward<Tuple>(tuple).template get<T>();
}

template <std::size_t I, concepts::Tuple... TuplesT>
constexpr decltype(auto) flat_get(TuplesT&&... tuples) {
  using FlatGetImplT = details::FlatGetImpl<TuplesT...>;
  using TupleOfTuplesT = FlatGetImplT::TupleOfTuplesT;

  TupleOfTuplesT tuple_of_tuples{tuples...};
  return FlatGetImplT::template Get<I>(tuple_of_tuples);
}

template <typename... Args>
constexpr auto tie(Args&... args) {
  using ResultType = Tuple<Args&...>;
  return ResultType{args...};
}

template <typename... Args>
constexpr auto make_tuple(Args&&... args) {
  using ResultType = Tuple<std::unwrap_ref_decay_t<Args>...>;
  return ResultType{std::forward<Args>(args)...};
}

template <concepts::Tuple... TuplesT>
constexpr auto tuple_cat(TuplesT&&... tuples) {
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