#pragma once

#include <cstddef>
#include <type_traits>

#include "../type_list.hpp"
#include "tuple_core.hpp"
#include "tuple_fwd.hpp"

namespace kxi::tuple::details {
template <concepts::Tuple... TuplesT>
struct FlatGetHelper {
  using TypeListOfAllTypes =
      type_list::CatListsT<type_list::TupleAsTypeListT<TuplesT>...>;
  static constexpr const std::size_t kNumberOfAllTypes =
      type_list::GetSizeV<TypeListOfAllTypes>;

  using TupleOfTuplesT = Tuple<std::remove_reference_t<TuplesT>&...>;

  template <std::size_t I>
  static constexpr decltype(auto) Get(TupleOfTuplesT& tuple_of_tuples) {
    static constexpr const auto kDestinationPos =
        type_list::GetDestinationPosV<I,
                                      type_list::TupleAsTypeListT<TuplesT>...>;

    using NecessaryTupleT = type_list::GetTypeT<type_list::TypeList<TuplesT...>,
                                                kDestinationPos.list_pos>;

    return std::forward<NecessaryTupleT>(
               tuple_of_tuples.template Get<kDestinationPos.list_pos>())
        .template Get<kDestinationPos.elem_pos>();
  }
};
};  // namespace kxi::tuple::details