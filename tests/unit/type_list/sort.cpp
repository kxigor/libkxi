#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/type_list.hpp>
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

TEST(TypeListSort, MappingArrayReversed) {
  // Input:  [0]=double(8), [1]=int(4), [2]=char(1)
  // Sorted: char, int, double => mapping [2, 1, 0]
  using Input = TypeList<double, int, char>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 3u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);
  EXPECT_EQ(SortInfo::kMappingArray[2], 0u);
}

TEST(TypeListSort, MappingArrayAlreadySorted) {
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
  // int(4), float(4) — equal sizeof, stable => [0, 1]
  using Input = TypeList<int, float>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray[0], 0u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 1u);
}

TEST(TypeListSort, MappingArrayFiveElements) {
  // Input:  [0]=double(8), [1]=int32(4), [2]=int8(1), [3]=float(4),
  // [4]=int16(2) Sorted: int8, int16, int32, float, double Mapping: [2, 4, 1,
  // 3, 0]
  using Input =
      TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kMappingArray.size(), 5u);
  EXPECT_EQ(SortInfo::kMappingArray[0], 2u);
  EXPECT_EQ(SortInfo::kMappingArray[1], 4u);
  EXPECT_EQ(SortInfo::kMappingArray[2], 1u);
  EXPECT_EQ(SortInfo::kMappingArray[3], 3u);
  EXPECT_EQ(SortInfo::kMappingArray[4], 0u);
}

// --- InverseMappingArray ---

TEST(TypeListSort, InverseMappingReversed) {
  // mapping = [2, 1, 0]
  // inverse: original[0] (double) ended at sorted position 2
  //          original[1] (int) ended at sorted position 1
  //          original[2] (char) ended at sorted position 0
  using Input = TypeList<double, int, char>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kInverseMappingArray.size(), 3u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[0], 2u);  // double -> pos 2
  EXPECT_EQ(SortInfo::kInverseMappingArray[1], 1u);  // int -> pos 1
  EXPECT_EQ(SortInfo::kInverseMappingArray[2], 0u);  // char -> pos 0
}

TEST(TypeListSort, InverseMappingAlreadySorted) {
  // mapping = [0, 1, 2] => inverse = [0, 1, 2]
  using Input = TypeList<char, int, double>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kInverseMappingArray[0], 0u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[1], 1u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[2], 2u);
}

TEST(TypeListSort, InverseMappingSingleElement) {
  using Input = TypeList<int>;
  using SortInfo = Sort<SizeofLess, Input>;
  EXPECT_EQ(SortInfo::kInverseMappingArray.size(), 1u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[0], 0u);
}

TEST(TypeListSort, InverseMappingFiveElements) {
  // mapping = [2, 4, 1, 3, 0]
  // inverse[0] = 4  (double was at input 0, now at sorted pos 4)
  // inverse[1] = 2  (int32 was at input 1, now at sorted pos 2)
  // inverse[2] = 0  (int8 was at input 2, now at sorted pos 0)
  // inverse[3] = 3  (float was at input 3, now at sorted pos 3)
  // inverse[4] = 1  (int16 was at input 4, now at sorted pos 1)
  using Input =
      TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<SizeofLess, Input>;

  EXPECT_EQ(SortInfo::kInverseMappingArray.size(), 5u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[0], 4u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[1], 2u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[2], 0u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[3], 3u);
  EXPECT_EQ(SortInfo::kInverseMappingArray[4], 1u);
}

TEST(TypeListSort, InverseIsInverseOfMapping) {
  // For any i: inverse[mapping[i]] == i
  using Input =
      TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using SortInfo = Sort<SizeofLess, Input>;

  for (std::size_t i = 0; i < SortInfo::kMappingArray.size(); ++i) {
    EXPECT_EQ(SortInfo::kInverseMappingArray[SortInfo::kMappingArray[i]], i);
  }
  // And: mapping[inverse[i]] == i
  for (std::size_t i = 0; i < SortInfo::kInverseMappingArray.size(); ++i) {
    EXPECT_EQ(SortInfo::kMappingArray[SortInfo::kInverseMappingArray[i]], i);
  }
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
  using Input =
      TypeList<double, std::int32_t, std::int8_t, float, std::int16_t>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_TRUE((std::is_same_v<Sorted, TypeList<std::int8_t, std::int16_t,
                                               std::int32_t, float, double>>));
}

TEST(TypeListSort, SortPreservesListSize) {
  using Input = TypeList<double, char, int, float, std::int64_t>;
  using Sorted = Sort<SizeofLess, Input>::SortedTypeListT;
  EXPECT_EQ(GetSizeV<Sorted>, 5u);
  EXPECT_EQ(GetSizeV<Sorted>, GetSizeV<Input>);
}

}  // namespace