#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>     // IWYU pragma: keep
#include <libkxi/utility.hpp>  // IWYU pragma: keep
#include <type_traits>
#include <utility>

#include "concepts.hpp"
#include "ops_details.hpp"

namespace kxi::tuple::like {
template <std::size_t I, concepts::TupleLike TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward_like<TupleT>(
      std::forward<TupleT>(tuple).template Get<I>());
}

template <typename T, concepts::DistinctTupleLike TupleT>
constexpr decltype(auto) Get(TupleT&& tuple) {
  return std::forward_like<TupleT>(
      std::forward<TupleT>(tuple).template Get<T>());
}

template <concepts::SwappableTuple TupleT>
constexpr void Swap(TupleT& lhs, TupleT& rhs) noexcept(
    noexcept(std::declval<TupleT&>().Swap(std::declval<TupleT&>()))) {
  lhs.Swap(rhs);
}

template <std::size_t I, concepts::TupleLike... TuplesT>
constexpr decltype(auto) SpanGet(TuplesT&&... tuples) {
  using FlatGetHelperT = details::SpanGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;

  RefsTupleT tuple_of_tuples{tuples...};
  return FlatGetHelperT::template Get<I>(tuple_of_tuples);
}

template <concepts::TupleLike... TuplesT>
requires meta::concepts::AllSameShells<TuplesT...>
constexpr auto Cat(TuplesT&&... tuples) {
  using FlatGetHelperT = details::SpanGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;
  using ResultType = meta::CatListsT<TuplesT...>;

  RefsTupleT tuple_of_tuples{tuples...};

  return
      [&]<std::size_t... Indexes>(std::index_sequence<Indexes...> /*unused*/) {
        return ResultType{
            FlatGetHelperT::template Get<Indexes>(tuple_of_tuples)...};
      }(std::make_index_sequence<meta::SizeV<ResultType>>{});
}

template <template <typename...> typename Shell, typename... Args>
constexpr auto Forward(Args&&... args) {
  using ResultType = Shell<Args&&...>;
  return ResultType{std::forward<Args>(args)...};
}

template <template <typename...> typename Shell, typename... Args>
constexpr auto ForwardInto(Args&&... args) {
  using ResultType = Shell<Args...>;
  return ResultType{std::forward<Args>(args)...};
}

template <template <typename...> typename Shell, typename... Args>
constexpr auto Tie(Args&... args) {
  using ResultType = Shell<Args&...>;
  return ResultType{args...};
}

template <template <typename...> typename Shell, typename... Args>
constexpr auto Make(Args&&... args) {
  using ResultType = Shell<std::unwrap_ref_decay_t<Args>...>;
  return ResultType{std::forward<Args>(args)...};
}

}  // namespace kxi::tuple::like