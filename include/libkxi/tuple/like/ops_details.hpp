#pragma once

#include <libkxi/meta.hpp>  // IWYU pragma: keep

#include "concepts.hpp"

namespace kxi::tuple::like::details {
template <concepts::TupleLike... TuplesT>
struct PackGetHelper {
  using MergedPackT = meta::CatListsT<TuplesT...>;
  using RefsTupleT = flat::FlatTuple<std::remove_reference_t<TuplesT>&...>;

  static constexpr auto kTotalSize = meta::SizeV<MergedPackT>;

  template <std::size_t I>
  static constexpr decltype(auto) Get(RefsTupleT& refs) {
    static constexpr auto kPos = meta::LocateIndexV<I, TuplesT...>;

    using TargetTupleT =
        meta::TypeAtT<kPos.list_pos, flat::FlatTuple<TuplesT...>>;

    return std::forward<TargetTupleT>(refs.template Get<kPos.list_pos>())
        .template Get<kPos.elem_pos>();
  }
};
}  // namespace kxi::tuple::like::details