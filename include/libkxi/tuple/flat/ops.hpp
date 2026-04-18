#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>
#include <libkxi/tuple/like.hpp>
#include <libkxi/types.hpp>
#include <type_traits>
#include <utility>

#include "concepts.hpp"
#include "core.hpp"
#include "fwd.hpp"

namespace kxi::tuple::flat {
template <typename... Args>
constexpr auto Forward(Args&&... args) {
  return FlatTuple<Args&&...>{std::forward<Args>(args)...};
}

template <typename... Args>
constexpr auto Tie(Args&... args) {
  using ResultType = FlatTuple<Args&...>;
  return ResultType{args...};
}

template <typename... Args>
constexpr auto Make(Args&&... args) {
  using ResultType = FlatTuple<std::unwrap_ref_decay_t<Args>...>;
  return ResultType{std::forward<Args>(args)...};
}

template <concepts::FlatTuple... TuplesT>
constexpr auto Cat(TuplesT&&... tuples) {
  using FlatGetHelperT = like::details::PackGetHelper<TuplesT...>;
  using RefsTupleT = FlatGetHelperT::RefsTupleT;
  using ResultType = FlatGetHelperT::MergedPackT;

  RefsTupleT tuple_of_tuples{tuples...};

  return
      [&]<std::size_t... Indexes>(std::index_sequence<Indexes...> /*unused*/) {
        return ResultType{
            FlatGetHelperT::template Get<Indexes>(tuple_of_tuples)...};
      }(std::make_index_sequence<FlatGetHelperT::kTotalSize>{});
}

}  // namespace kxi::tuple::flat