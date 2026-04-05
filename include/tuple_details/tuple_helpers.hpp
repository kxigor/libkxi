#pragma once

#include <cstddef>

#include "../type_list.hpp"
#include "tuple_concepts.hpp"
#include "tuple_fwd.hpp"

namespace kxi::tuple {
template <concepts::Tuple TupleT>
struct TupleSize {
  static constexpr const std::size_t value =
      type_list::GetSizeV<type_list::TupleAsTypeListT<TupleT>>;
};

template <concepts::Tuple TupleT>
constexpr const std::size_t TupleSizeV = TupleSize<TupleT>::value;

template <std::size_t I, concepts::Tuple TupleT>
struct TupleElement {
  using type = type_list::GetTypeT<type_list::TupleAsTypeListT<TupleT>, I>;
};

template <std::size_t I, concepts::Tuple TupleT>
using TupleElementT = TupleElement<I, TupleT>::type;

}  // namespace kxi::tuple