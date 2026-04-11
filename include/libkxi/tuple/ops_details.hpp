#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>
#include <libkxi/tuple/concepts.hpp>
#include <libkxi/tuple/core.hpp>
#include <libkxi/tuple/fwd.hpp>
#include <libkxi/types.hpp>
#include <type_traits>

namespace kxi::tuple::details {
template <concepts::FlatTuple... TuplesT>
struct FlatGetHelper {
  using MergedPackT = meta::CatListsT<FlatTuple, TuplesT...>;
  using RefsTupleT = FlatTuple<std::remove_reference_t<TuplesT>&...>;

  static constexpr auto kTotalSize = meta::GetSizeV<FlatTuple, MergedPackT>;

  template <std::size_t I>
  static constexpr decltype(auto) Get(RefsTupleT& refs) {
    static constexpr auto kPos =
        meta::GetDestinationPosV<FlatTuple, I, TuplesT...>;

    using TargetTupleT =
        meta::GetTypeT<FlatTuple, kPos.list_pos, FlatTuple<TuplesT...>>;

    return std::forward<TargetTupleT>(refs.template Get<kPos.list_pos>())
        .template Get<kPos.elem_pos>();
  }
};
};  // namespace kxi::tuple::details