#pragma once

#include <libkxi/sorted_tuple_details/fwd.hpp>
#include <libkxi/tuple.hpp>
#include <libkxi/type_list.hpp>

namespace kxi::sorted_tuple {

template <template <typename LHS, typename RHS> typename Predicat,
          typename... Types>
class SortedTuple {
 private:
  /*========================== Usings ==========================*/
  using SorterT = type_list::Sort<Predicat, type_list::TypeList<Types...>>;
  using BaseTupleT =
      type_list::TypeListAsTupleT<typename SorterT::SortedTypeListT>;
  using TypesTypeListT = type_list::TypeList<Types...>;

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
              using ArgsTypeListT = type_list::TypeList<Args...>;
              using SavedArgsTupleT =
                  tuple::Tuple<std::remove_reference_t<Args>&...>;

              SavedArgsTupleT tuple_args{args...};

              return BaseTupleT{std::forward<type_list::GetTypeT<
                  SorterT::kMappingArray[Is], ArgsTypeListT>>(
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

};  // namespace kxi::sorted_tuple