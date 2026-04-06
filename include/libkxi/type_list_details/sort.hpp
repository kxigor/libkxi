#pragma once

#include <array>
#include <cstddef>
#include <libkxi/type_list_details/concepts.hpp>
#include <libkxi/type_list_details/core.hpp>
#include <libkxi/type_list_details/fwd.hpp>
#include <libkxi/utility.hpp>
#include <numeric>

namespace kxi::type_list {

template <template <typename LHS, typename RHS> typename PredicatT,
          concepts::TypeList TypeListT>
struct Sort {
 public:
  static constexpr const std::size_t kNumOfTypes = GetSizeV<TypeListT>;
  using MappingArrayT = std::array<std::size_t, kNumOfTypes>;
  using GivenTypeListT = TypeListT;

  template <typename T, std::size_t Size>
  using CompareTableContainerT = std::array<T, Size>;
  using CompareLineT = CompareTableContainerT<bool, kNumOfTypes>;
  using CompareTableT = CompareTableContainerT<CompareLineT, kNumOfTypes>;

 private:
  [[nodiscard]] static constexpr auto GetCompareTable() noexcept {
    return []<std::size_t... Is>(std::index_sequence<Is...>) {
      return CompareTableT{([]<std::size_t Idx>() {
        return CompareLineT{PredicatT<GetTypeT<Idx, GivenTypeListT>,
                                      GetTypeT<Is, GivenTypeListT>>::value...};
      }.template operator()<Is>())...};
    }(std::make_index_sequence<kNumOfTypes>{});
  }

 public:
  static constexpr const auto kCompareTable = GetCompareTable();

 private:
  [[nodiscard]] static constexpr MappingArrayT GetMappingArray() noexcept {
    MappingArrayT result;
    std::ranges::iota(result, 0);

    for (std::size_t i = 1; i < result.size(); ++i) {
      for (std::size_t j = 0; j < i; ++j) {
        const std::size_t kLhsIdx = i - j - 1;
        const std::size_t kRhsIdx = i - j;
        if (not kCompareTable[result[kRhsIdx]][result[kLhsIdx]]) {
          break;
        }
        std::swap(result[kLhsIdx], result[kRhsIdx]);
      }
    }
    return result;
  }

 public:
  static constexpr const auto kMappingArray = GetMappingArray();

 private:
  [[nodiscard]] static constexpr MappingArrayT
  GetInverseMappingArray() noexcept {
    MappingArrayT result{};
    for (std::size_t i = 0; i < kMappingArray.size(); ++i) {
      result[kMappingArray[i]] = i;
    }
    return result;
  }

 public:
  static constexpr const auto kInverseMappingArray = GetInverseMappingArray();

  using SortedTypeListT =
      decltype([]<std::size_t... Is>(std::index_sequence<Is...>) {
        return TypeList<GetTypeT<kMappingArray[Is], GivenTypeListT>...>{};
      }(std::make_index_sequence<kNumOfTypes>{}));
};

}  // namespace kxi::type_list