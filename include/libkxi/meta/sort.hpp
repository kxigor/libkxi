#pragma once

#include <array>
#include <cstddef>
#include <libkxi/meta/concepts.hpp>
#include <libkxi/meta/core.hpp>
#include <libkxi/utility.hpp>
#include <numeric>

namespace kxi::meta {

template <template <typename LHS, typename RHS> typename PredicatT,
          concepts::VariadicTemplate TList>
struct Sort;

template <template <typename...> typename Shell,
          template <typename LHS, typename RHS> typename PredicatT,
          typename... Args>
struct Sort<PredicatT, Shell<Args...>> {
 public:
  using TList = Shell<Args...>;
  static constexpr const std::size_t kSize = SizeV<TList>;
  using RowT = std::array<bool, kSize>;
  using MatrixT = std::array<RowT, kSize>;
  using IndexMapT = std::array<std::size_t, kSize>;

 private:
  [[nodiscard]] static constexpr auto MakeRelationMatrix() noexcept {
    return []<std::size_t... Is>(std::index_sequence<Is...>) {
      return MatrixT{([]<std::size_t Idx>() {
        return RowT{PredicatT<meta::TypeAtT<Idx, TList>,
                              meta::TypeAtT<Is, TList>>::value...};
      }.template operator()<Is>())...};
    }(std::make_index_sequence<kSize>{});
  }

 public:
  static constexpr const auto kRelations = MakeRelationMatrix();

 private:
  [[nodiscard]] static constexpr IndexMapT MakePermutation() noexcept {
    IndexMapT result;
    std::ranges::iota(result, 0);

    for (std::size_t i = 1; i < result.size(); ++i) {
      for (std::size_t j = 0; j < i; ++j) {
        const std::size_t kLhsIdx = i - j - 1;
        const std::size_t kRhsIdx = i - j;
        if (not kRelations[result[kRhsIdx]][result[kLhsIdx]]) {
          break;
        }
        std::swap(result[kLhsIdx], result[kRhsIdx]);
      }
    }
    return result;
  }

 public:
  static constexpr const auto kPermutation = MakePermutation();

 private:
  [[nodiscard]] static constexpr IndexMapT MakeInversePermutation() noexcept {
    IndexMapT result{};
    for (std::size_t i = 0; i < kPermutation.size(); ++i) {
      result[kPermutation[i]] = i;
    }
    return result;
  }

 public:
  static constexpr const auto kInversePermutation = MakeInversePermutation();

  using SortedShellT =
      decltype([]<std::size_t... Is>(std::index_sequence<Is...>) {
        return Shell<meta::TypeAtT<kPermutation[Is], TList>...>{};
      }(std::make_index_sequence<kSize>{}));
};

}  // namespace kxi::meta