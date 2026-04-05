#pragma once

#include <libkxi/sorted_tuple_details/fwd.hpp>
#include <libkxi/tuple.hpp>
#include <libkxi/type_list.hpp>

namespace kxi::sorted_tuple {

template <template <typename LHS, typename RHS> typename Predicat,
          typename... Types>
class SortedTuple {
 private:
  using SorterT = type_list::Sort<Predicat, type_list::TypeList<Types...>>;
  using SortedTupleT = type_list::TypeListAsTupleT<SorterT::SortedTypeListT>;

 public:
  constexpr SortedTuple() = default;

  constexpr SortedTuple(const SortedTuple& /*unused*/) = default;

  constexpr SortedTuple(SortedTuple&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<SortedTuple, Args...>
  constexpr SortedTuple(Args&&... args) : tuple_(/*prvalue labmda here*/) {}

  constexpr ~SortedTuple() = default;

  constexpr SortedTuple& operator=(const SortedTuple& /*unused*/) = default;

  constexpr SortedTuple& operator=(SortedTuple&& /*unused*/) = default;

 private:
  SortedTupleT tuple_;
};

};  // namespace kxi::sorted_tuple