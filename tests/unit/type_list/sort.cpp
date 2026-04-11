#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/heterogeneous/sort.hpp>
#include <type_traits>

namespace {

using kxi::het::GetSizeV;
using kxi::het::Sort;

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
  using SortInfo = Sort<TList, SizeofLess, Input>;

  EXPECT_FALSE(SortInfo::kCompareTable[0][0]);
  EXPECT_TRUE(SortInfo::kCompareTable[0][1]);
  EXPECT_TRUE(SortInfo::kCompareTable[0][2]);

  EXPECT_FALSE(SortInfo::kCompareTable[1][0]);
  EXPECT_FALSE(SortInfo::kCompareTable[1][1]);
  EXPECT_TRUE(SortInfo::kCompareTable[1][2]);

  EXPECT_FALSE(SortInfo::kCompareTable[2][0]);
  EXPECT_FALSE(SortInfo::kCompareTable[2][1]);
  EXPECT_FALSE(SortInfo::kCompareTable[2][2]);
}

// --- MappingArray ---

TEST(HeterogeneousSort, MappingArrayReversed) {
  using Input = TList<double, int, char>;
  using SortInfo = Sort<TList, SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 3u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);
  EXPECT_EQ(SortInfo::kMappingArray[2], 0u);
}

TEST(HeterogeneousSort, MappingArrayFiveElements) {
  using Input = TList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<TList, SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 5u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 4u);
  EXPECT_EQ(SortInfo::kMappingArray[2], 1u);
  EXPECT_EQ(SortInfo::kMappingArray[3], 3u);
  EXPECT_EQ(SortInfo::kMappingArray[4], 0u);
}

// --- InverseMappingArray ---

TEST(HeterogeneousSort, InverseMappingReversed) {
  using Input = TList<double, int, char>;
  using SortInfo = Sort<TList, SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kInverseMappingArray.size(), 3u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[0], 2u); 
  EXPECT_EQ(SortInfo::kInverseMappingArray[1], 1u); 
  EXPECT_EQ(SortInfo::kInverseMappingArray[2], 0u); 
}

// --- SortedTypeListT ---

TEST(HeterogeneousSort, AscendingBySizeof) {
  using Input = TList<double, char, int>;
  using Sorted = Sort<TList, SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<char, int, double>>));
}

TEST(HeterogeneousSort, DescendingBySizeof) {
  using Input = TList<char, double, int>;
  using Sorted = Sort<TList, SizeofGreater, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<double, int, char>>));
}

TEST(HeterogeneousSort, StabilityEqualSizes) {
  using Input = TList<int, float>;
  using Sorted = Sort<TList, SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TList<int, float>>));
}

}  // namespace