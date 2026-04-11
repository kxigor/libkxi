#pragma once

#include <libkxi/heterogeneous.hpp>
#include <libkxi/sorted_tuple/fwd.hpp>
#include <libkxi/tuple.hpp>

namespace kxi::tuple {

template <template <typename LHS, typename RHS> typename Predicat,
          typename... Types>
class SortedTuple {
 private:
  /*========================== Usings ==========================*/
  using SorterT = het::Sort<FlatTuple, Predicat, FlatTuple<Types...>>;
  using BaseTupleT = typename SorterT::SortedTypeListT;

 public:
  /*================= Constructors/Destructors =================*/
  constexpr SortedTuple() = default;

  constexpr SortedTuple(const SortedTuple& /*unused*/) = default;

  constexpr SortedTuple(SortedTuple&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<SortedTuple, Args...> &&
           (sizeof...(Args) == sizeof...(Types))
  constexpr SortedTuple(Args&&... args)
      : tuple_(
            /*copy elision works here*/
            [&]<std::size_t... Is>(std::index_sequence<Is...> /*unused*/) {
              using ArgsTypeListT = het::Types<Args...>;
              using SavedArgsTupleT =
                  tuple::FlatTuple<std::remove_reference_t<Args>&...>;

              SavedArgsTupleT tuple_args{args...};

              return BaseTupleT{std::forward<het::GetTypeT<
                  het::Types, SorterT::kMappingArray[Is], ArgsTypeListT>>(
                  tuple_args.template Get<SorterT::kMappingArray[Is]>())...};
            }(std::make_index_sequence<sizeof...(Args)>{})) {}

  constexpr ~SortedTuple() = default;

  /*======================= Assignments ========================*/
  constexpr SortedTuple& operator=(const SortedTuple& /*unused*/) = default;

  constexpr SortedTuple& operator=(SortedTuple&& /*unused*/) = default;

  /*======================== Interface =========================*/
  template <std::size_t I, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward_like<Self>(self.tuple_)
        .template Get<SorterT::kInverseMappingArray[I]>();
  }

  template <typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward_like<Self>(self.tuple_).template Get<T>();
  }

  constexpr void Swap(SortedTuple& other) noexcept(
      noexcept(std::declval<BaseTupleT&>().Swap(std::declval<BaseTupleT&>()))) {
    tuple_.Swap(other.tuple_);
  }

 private:
  /*======================= Data fields ========================*/
  BaseTupleT tuple_;
};

};  // namespace kxi::tuple