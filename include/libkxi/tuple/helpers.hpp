#pragma once

#include <libkxi/heterogeneous.hpp>
#include <libkxi/tuple/concepts.hpp>
#include <libkxi/tuple/fwd.hpp>
#include <libkxi/types.hpp>

namespace kxi::tuple {
template <concepts::FlatTuple TupleT>
struct TupleSize : het::GetSize<FlatTuple, TupleT> {};

template <concepts::FlatTuple TupleT>
constexpr const std::size_t TupleSizeV = TupleSize<TupleT>::value;

template <std::size_t I, concepts::FlatTuple TupleT>
struct TupleElement : het::GetType<FlatTuple, I, TupleT> {
  using typename het::GetType<FlatTuple, I, TupleT>::type;
};

template <std::size_t I, concepts::FlatTuple TupleT>
using TupleElementT = TupleElement<I, TupleT>::type;

}  // namespace kxi::tuple