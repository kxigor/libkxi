#pragma once

#include <cstddef>
#include <libkxi/heterogeneous.hpp>
#include <libkxi/tuple/concepts.hpp>
#include <libkxi/tuple/core.hpp>
#include <libkxi/tuple/fwd.hpp>
#include <libkxi/types.hpp>
#include <type_traits>

namespace kxi::tuple::details {
template <concepts::FlatTuple... TuplesT>
struct FlatGetHelper {
  using TypeListOfAllTypes = het::CatListsT<FlatTuple, TuplesT...>;
  static constexpr const std::size_t kNumberOfAllTypes =
      het::GetSizeV<FlatTuple, TypeListOfAllTypes>;

  using TupleOfTuplesT = FlatTuple<std::remove_reference_t<TuplesT>&...>;

  template <std::size_t I>
  static constexpr decltype(auto) Get(TupleOfTuplesT& tuple_of_tuples) {
    static constexpr const auto kDestinationPos =
        het::GetDestinationPosV<FlatTuple, I, TuplesT...>;

    using NecessaryTupleT = het::GetTypeT<FlatTuple, kDestinationPos.list_pos,
                                          FlatTuple<TuplesT...>>;

    return std::forward<NecessaryTupleT>(
               tuple_of_tuples.template Get<kDestinationPos.list_pos>())
        .template Get<kDestinationPos.elem_pos>();
  }
};
};  // namespace kxi::tuple::details