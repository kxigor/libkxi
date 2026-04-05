#include <gtest/gtest.h>
#include <libkxi/type_list.hpp>
#include <cstdint>
#include <type_traits>

namespace {

using kxi::type_list::GetSizeV;
using kxi::type_list::Sort;
using kxi::type_list::TypeList;

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

template <typename LHS, typename RHS>
struct SizeofGreater {
  static constexpr const bool value = sizeof(LHS) > sizeof(RHS);
};

// --- CompareTable ---

TEST(TypeListSort, CompareTableValues) {
  // char(1), int(4), double(8)
  using Input = TypeList<char, int, double>;
  using SortInfo = Sort<SizeofLess, Input>;

  // table[i][j] = sizeof(T_i) < sizeof(T_j)
  // [0][0]: char < char  = false
  // [0][1]: char < int   = true
  // [0][2]: char < double= true
  EXPECT_FALSE(SortInfo::kCompareTable[0][0]);
  EXPECT_TRUE(SortInfo::kCompareTable[0][1]);
  EXPECT_TRUE(SortInfo::kCompareTable[0][2]);

  // [1][0]: int < char   = false
  // [1][1]: int < int    = false
  // [1][2]: int < double = true
  EXPECT_FALSE(SortInfo::kCompareTable[1][0]);
  EXPECT_FALSE(SortInfo::kCompareTable[1][1]);
  EXPECT_TRUE(SortInfo::kCompareTable[1][2]);

  // [2][0]: double < char = false
  // [2][1]: double < int  = false
  // [2][2]: double < double = false
  EXPECT_FALSE(SortInfo::kCompareTable[2][0]);
  EXPECT_FALSE(SortInfo::kCompareTable[2][1]);
  EXPECT_FALSE(SortInfo::kCompareTable[2][2]);
}

// --- MappingArray ---

TEST(TypeListSort, MappingArrayReversed) {
  // double(8), int(4), char(1) -> sorted ascending: char, int, double
  // original indices: char=2, int=1, double=0
  // mapping should be [2, 1, 0]
  using Input = TypeList<double, int, char>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 3u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);  // char was at index 2
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);  // int was at index 1
  EXPECT_EQ(SortInfo::kMappingArray[2], 0u);  // double was at index 0
}

TEST(TypeListSort, MappingArrayAlreadySorted) {
  // char(1), int(4), double(8) — already sorted ascending
  // mapping should be identity [0, 1, 2]
  using Input = TypeList<char, int, double>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray[0], 0u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);
  EXPECT_EQ(SortInfo::kMappingArray[2], 2u);
}

TEST(TypeListSort, MappingArraySingleElement) {
  using Input = TypeList<int>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 1u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 0u);
}

TEST(TypeListSort, MappingArrayStabilityOnEqualSizes) {
  // int(4), float(4) — equal sizeof, predicate always false
  // insertion sort is stable => original order preserved => [0, 1]
  using Input = TypeList<int, float>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray[0], 0u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);
}

// --- SortedTypeListT ---

TEST(TypeListSort, AscendingBySizeof) {
  using Input = TypeList<double, char, int>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<char, int, double>>));
}

TEST(TypeListSort, DescendingBySizeof) {
  using Input = TypeList<char, double, int>;
  using Sorted = Sort<SizeofGreater, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<double, int, char>>));
}

TEST(TypeListSort, AlreadySortedIsNoop) {
  using Input = TypeList<char, int, double>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, Input>));
}

TEST(TypeListSort, SingleElementSort) {
  using Input = TypeList<int>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<int>>));
}

TEST(TypeListSort, StabilityEqualSizes) {
  using Input = TypeList<int, float>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<int, float>>));
}
TEST(TypeListSort, FiveElementsAscending) {
  // std::int8_t(1), std::int16_t(2), float(4), std::int32_t(4), double(8)
  using Input = TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;

  // Expected: int8(1), int16(2), then int32 and float (both 4, stable order: int32 before float), double(8)
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<std::int8_t, std::int16_t, std::int32_t, float, double>>));
}

TEST(TypeListSort, SortPreservesListSize) {
  using Input = TypeList<double, char, int, float, std::int64_t>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_EQ(GetSizeV<Sorted>, 5u);
  EXPECT_EQ(GetSizeV<Sorted>, GetSizeV<Input>);
}

TEST(TypeListSort, MappingArrayFiveElements) {
  // Input:  [0]=double(8), [1]=int32(4), [2]=int8(1), [3]=float(4), [4]=int16(2)
  // Sorted: int8, int16, int32, float, double
  // Mapping: [2, 4, 1, 3, 0]
  using Input = TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 5u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);  // int8 was at 2
  EXPECT_EQ(SortInfo::kMappingArray[1], 4u);  // int16 was at 4
  EXPECT_EQ(SortInfo::kMappingArray[2], 1u);  // int32 was at 1
  EXPECT_EQ(SortInfo::kMappingArray[3], 3u);  // float was at 3
  EXPECT_EQ(SortInfo::kMappingArray[4], 0u);  // double was at 0
}

}  // namespace
