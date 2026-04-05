#pragma once

#include <array>
#include <cstddef>
#include <libkxi/type_list_details/concepts.hpp>
#include <libkxi/type_list_details/core.hpp>
#include <libkxi/type_list_details/fwd.hpp>
#include <libkxi/utility.hpp>
#include <numeric>

namespace kxi::type_list {

namespace concepts {
template <typename T>
concept PredicatResult = requires {
  { T::value } -> std::convertible_to<bool>;
} && std::is_same_v<const bool, decltype(T::value)>;
}  // namespace concepts

template <template <typename LHS, typename RHS> typename PredicatT,
          concepts::TypeList TypeListT,
          utility::concepts::IndexSequence IndexSequenceT>
struct SortImpl;

template <template <typename LHS, typename RHS> typename PredicatT,
          typename... Types, std::size_t... Indexes>
struct SortImpl<PredicatT, TypeList<Types...>,
                std::index_sequence<Indexes...>> {
 public:
  static constexpr const std::size_t kNumOfTypes = sizeof...(Types);
  using MappingArrayT = std::array<std::size_t, kNumOfTypes>;
  using GivenTypeListT = TypeList<Types...>;

  /*
    TODO: возможно если типов много, а памяти мало
    лучше использовать std::vector, добавить conditional
  */
  template <typename T, std::size_t Size>
  using CompareTableContainerT = std::array<T, Size>;
  using CompareLineT = CompareTableContainerT<bool, kNumOfTypes>;
  using CompareTableT = CompareTableContainerT<CompareLineT, kNumOfTypes>;

 private:
  static constexpr auto GetCompareTable() {
    return []<std::size_t... Is>(std::index_sequence<Is...> /*unused*/) {
      return CompareTableT{([]<std::size_t Idx>() {
        return CompareLineT{PredicatT<GetTypeT<Idx, GivenTypeListT>,
                                      GetTypeT<Is, GivenTypeListT>>::value...};
      }.template operator()<Is>())...};
    }(std::make_index_sequence<kNumOfTypes>{});
  }

 public:
  static constexpr const auto kCompareTable = GetCompareTable();

 private:
  static constexpr MappingArrayT GetMappingArray() {
    MappingArrayT result;
    std::ranges::iota(result, 0);

    for (std::size_t i = 1; i < result.size(); ++i) {
      for (std::size_t j = 0; j < i; ++j) {
        const std::size_t kLhsIdx = i - j - 1;
        const std::size_t kRhsIdx = i - j;
        if (kCompareTable[result[kLhsIdx]][result[kRhsIdx]]) {
          break;
        }
        std::swap(result[kLhsIdx], result[kRhsIdx]);
      }
    }

    return result;
  }

 public:
  static constexpr const auto kMappingArray = GetMappingArray();
  using SortedTypeListT =
      TypeList<GetTypeT<kMappingArray[Indexes], GivenTypeListT>...>;
};

template <template <typename LHS, typename RHS> typename PredicatT,
          concepts::TypeList TypeListT>
struct Sort : SortImpl<PredicatT, TypeListT,
                       std::make_index_sequence<GetSizeV<TypeListT>>> {};

template <typename LHS, typename RHS>
struct Predicat {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

Sort<Predicat, TypeList<int, char, int>> a{};
}  // namespace kxi::type_list
