#pragma once

#include <cstddef>
#include <libkxi/tuple_details/concepts.hpp>
#include <libkxi/tuple_details/fwd.hpp>
#include <libkxi/type_list.hpp>

namespace kxi::tuple {
template <concepts::Tuple TupleT>
struct TupleSize : type_list::GetSize<type_list::TupleAsTypeListT<TupleT>> {};

template <concepts::Tuple TupleT>
constexpr const std::size_t TupleSizeV = TupleSize<TupleT>::value;

template <std::size_t I, concepts::Tuple TupleT>
struct TupleElement
    : type_list::GetType<I, type_list::TupleAsTypeListT<TupleT>> {};

template <std::size_t I, concepts::Tuple TupleT>
using TupleElementT = TupleElement<I, TupleT>::type;

}  // namespace kxi::tuple