#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/meta/sort.hpp>
#include <type_traits>

namespace {

using kxi::meta::GetSizeV;
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

}  // namespace