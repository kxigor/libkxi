#pragma once

#include <cstddef>
#include <libkxi/meta.hpp>

#include "concepts.hpp"

namespace kxi::tuple::flat {
template <concepts::Tuple TupleT>
struct TupleSize : meta::Size<TupleT> {};

template <concepts::Tuple TupleT>
constexpr const std::size_t TupleSizeV = TupleSize<TupleT>::value;

template <std::size_t I, concepts::Tuple TupleT>
struct TupleElement : meta::TypeAt<I, TupleT> {
  using typename meta::TypeAt<I, TupleT>::type;
};

template <std::size_t I, concepts::Tuple TupleT>
using TupleElementT = TupleElement<I, TupleT>::type;

}  // namespace kxi::tuple::flat