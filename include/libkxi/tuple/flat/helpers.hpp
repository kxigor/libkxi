#pragma once

#include <libkxi/meta.hpp>
#include <libkxi/types.hpp>

#include "concepts.hpp"
#include "fwd.hpp"

namespace kxi::tuple::flat {
template <concepts::FlatTuple TupleT>
struct TupleSize : meta::Size<TupleT> {};

template <concepts::FlatTuple TupleT>
constexpr const std::size_t TupleSizeV = TupleSize<TupleT>::value;

template <std::size_t I, concepts::FlatTuple TupleT>
struct TupleElement : meta::TypeAt<I, TupleT> {
  using typename meta::TypeAt<I, TupleT>::type;
};

template <std::size_t I, concepts::FlatTuple TupleT>
using TupleElementT = TupleElement<I, TupleT>::type;

}  // namespace kxi::tuple::flat