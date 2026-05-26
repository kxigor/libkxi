#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/meta/sort.hpp>
#include <type_traits>

namespace {

using kxi::meta::SizeV;
using kxi::meta::Sort;

template <typename...>
struct TList {};

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

template <typename LHS, typename RHS>
struct SizeofGreater {
  static constexpr const bool value = sizeof(LHS) > sizeof(RHS);
};

// --- CompareTable ---

TEST(HeterogeneousSort, CompareTableValues) {
  using Input = TList<char, int, double>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_FALSE(SortInfo::kRelations[0][0]);
  EXPECT_TRUE(SortInfo::kRelations[0][1]);
  EXPECT_TRUE(SortInfo::kRelations[0][2]);

  EXPECT_FALSE(SortInfo::kRelations[1][0]);
  EXPECT_FALSE(SortInfo::kRelations[1][1]);
  EXPECT_TRUE(SortInfo::kRelations[1][2]);

  EXPECT_FALSE(SortInfo::kRelations[2][0]);
  EXPECT_FALSE(SortInfo::kRelations[2][1]);
  EXPECT_FALSE(SortInfo::kRelations[2][2]);
}

// --- MappingArray ---

TEST(HeterogeneousSort, MappingArrayReversed) {
  using Input = TList<double, int, char>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kPermutation.size(), 3u);
  EXPECT_EQ(SortInfo::kPermutation[0], 2u);
  EXPECT_EQ(SortInfo::kPermutation[1], 1u);
  EXPECT_EQ(SortInfo::kPermutation[2], 0u);
}

TEST(HeterogeneousSort, MappingArrayFiveElements) {
  using Input = TList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kPermutation.size(), 5u);
  EXPECT_EQ(SortInfo::kPermutation[0], 2u);
  EXPECT_EQ(SortInfo::kPermutation[1], 4u);
  EXPECT_EQ(SortInfo::kPermutation[2], 1u);
  EXPECT_EQ(SortInfo::kPermutation[3], 3u);
  EXPECT_EQ(SortInfo::kPermutation[4], 0u);
}

// --- InverseMappingArray ---

TEST(HeterogeneousSort, InverseMappingReversed) {
  using Input = TList<double, int, char>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kInversePermutation.size(), 3u);
  EXPECT_EQ(SortInfo::kInversePermutation[0], 2u);
  EXPECT_EQ(SortInfo::kInversePermutation[1], 1u);
  EXPECT_EQ(SortInfo::kInversePermutation[2], 0u);
}

// --- SortedShellT ---

TEST(HeterogeneousSort, AscendingBySizeof) {
  using Input = TList<double, char, int>;
  using Sorted = Sort<SizeofLess, Input>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<char, int, double>>));
}

TEST(HeterogeneousSort, DescendingBySizeof) {
  using Input = TList<char, double, int>;
  using Sorted = Sort<SizeofGreater, Input>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<double, int, char>>));
}

TEST(HeterogeneousSort, StabilityEqualSizes) {
  using Input = TList<int, float>;
  using Sorted = Sort<SizeofLess, Input>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<int, float>>));
}

// --- Edge cases ---

TEST(HeterogeneousSort, EmptyList) {
  using Sorted = Sort<SizeofLess, TList<>>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<>>));
  EXPECT_EQ((Sort<SizeofLess, TList<>>::kPermutation.size()), 0u);
}

TEST(HeterogeneousSort, SingleElement) {
  using Sorted = Sort<SizeofLess, TList<int>>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<int>>));
  using Info = Sort<SizeofLess, TList<int>>;
  EXPECT_EQ(Info::kPermutation[0], 0u);
  EXPECT_EQ(Info::kInversePermutation[0], 0u);
}

TEST(HeterogeneousSort, AllEqualKeepsOriginalOrder) {
  // Stable sort: equal-sized types must preserve original order.
  using Input = TList<int, float, std::int32_t>;  // all 4 bytes
  using Sorted = Sort<SizeofLess, Input>::SortedShellT;
  EXPECT_TRUE((std::is_same_v<Sorted, Input>));
}

TEST(HeterogeneousSort, InversePermutationIsInverse) {
  using Input = TList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using Info = Sort<SizeofLess, Input>;
  for (std::size_t i = 0; i < Info::kPermutation.size(); ++i) {
    EXPECT_EQ(Info::kInversePermutation[Info::kPermutation[i]], i);
    EXPECT_EQ(Info::kPermutation[Info::kInversePermutation[i]], i);
  }
}

TEST(HeterogeneousSort, PreservesShell) {
  // Sort must keep the same shell template, only reorder type arguments.
  using Sorted = Sort<SizeofLess, TList<double, char>>::SortedShellT;
  using kxi::meta::IsSameShellV;
  EXPECT_TRUE((IsSameShellV<Sorted, TList<int>>));
}

}  // namespace