#pragma once

#include <libkxi/tuple_details/concepts.hpp>
#include <libkxi/tuple_details/fwd.hpp>
#include <libkxi/type_list_details/concepts.hpp>
#include <libkxi/type_list_details/fwd.hpp>

namespace kxi::type_list {

namespace details {
template <typename TupleT>
struct TupleAsTypeListImpl;

template <typename... TupleTypesT>
struct TupleAsTypeListImpl<kxi::tuple::Tuple<TupleTypesT...>> {
  using type = TypeList<TupleTypesT...>;
};
}  // namespace details

template <typename TupleT>
struct TupleAsTypeList {
  using type = details::TupleAsTypeListImpl<std::remove_cvref_t<TupleT>>::type;
};

template <typename TupleT>
using TupleAsTypeListT = TupleAsTypeList<TupleT>::type;

template <typename TypeListT>
struct TypeListAsTuple;

template <typename... TypeListTypesT>
struct TypeListAsTuple<TypeList<TypeListTypesT...>> {
  using type = kxi::tuple::Tuple<TypeListTypesT...>;
};

template <typename TypeListT>
using TypeListAsTupleT = TypeListAsTuple<TypeListT>::type;

}  // namespace kxi::type_list