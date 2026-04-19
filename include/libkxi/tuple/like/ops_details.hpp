#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>  // IWYU pragma: keep

#include "concepts.hpp"

namespace kxi::tuple::like::details {
template <concepts::TupleLike... TuplesT>
struct SpanGetHelper {
  using FirstShellT = meta::ShellOfT<meta::pack::TypeAtT<0, TuplesT...>>;
  using RefsTupleT = typename FirstShellT::template type<TuplesT&...>;

  template <std::size_t I>
  [[nodiscard]] static constexpr decltype(auto) Get(RefsTupleT& refs) {
    static constexpr auto kPos = meta::LocateIndexV<I, TuplesT...>;

    using TargetTupleT = meta::pack::TypeAtT<kPos.list_pos, TuplesT...>;

    return std::forward<TargetTupleT>(refs.template Get<kPos.list_pos>())
        .template Get<kPos.elem_pos>();
  }
};
}  // namespace kxi::tuple::like::details