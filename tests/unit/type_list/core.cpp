#include <gtest/gtest.h>

#include <libkxi/heterogeneous.hpp>
#include <type_traits>

namespace {

using kxi::het::CatListsT;
using kxi::het::GetCount;
using kxi::het::GetDestinationPosV;
using kxi::het::GetIndexV;
using kxi::het::GetSizeV;
using kxi::het::GetTypeT;

template <typename...>
struct TList {};

// --- GetSize ---

TEST(HeterogeneousGetSize, EmptyList) {
  EXPECT_EQ((GetSizeV<TList, TList<>>), 0u);
}

TEST(HeterogeneousGetSize, SingleElement) {
  EXPECT_EQ((GetSizeV<TList, TList<int>>), 1u);
}

TEST(HeterogeneousGetSize, MultipleElements) {
  EXPECT_EQ((GetSizeV<TList, TList<int, double, char>>), 3u);
}

// --- GetType ---

TEST(HeterogeneousGetType, FirstElement) {
  EXPECT_TRUE(
      (std::is_same_v<GetTypeT<TList, 0, TList<int, double, char>>, int>));
}

TEST(HeterogeneousGetType, MiddleElement) {
  EXPECT_TRUE(
      (std::is_same_v<GetTypeT<TList, 1, TList<int, double, char>>, double>));
}

TEST(HeterogeneousGetType, LastElement) {
  EXPECT_TRUE(
      (std::is_same_v<GetTypeT<TList, 2, TList<int, double, char>>, char>));
}

TEST(HeterogeneousGetType, SingleElementList) {
  EXPECT_TRUE((std::is_same_v<GetTypeT<TList, 0, TList<float>>, float>));
}

// --- GetCount ---

TEST(HeterogeneousGetCount, TypeNotPresent) {
  EXPECT_EQ((GetCount<TList, float, TList<int, double, char>>::value), 0u);
}

TEST(HeterogeneousGetCount, TypePresentOnce) {
  EXPECT_EQ((GetCount<TList, int, TList<int, double, char>>::value), 1u);
}

TEST(HeterogeneousGetCount, TypePresentMultipleTimes) {
  EXPECT_EQ((GetCount<TList, int, TList<int, double, int, char, int>>::value),
            3u);
}

TEST(HeterogeneousGetCount, EmptyList) {
  EXPECT_EQ((GetCount<TList, int, TList<>>::value), 0u);
}

// --- GetIndex ---

TEST(HeterogeneousGetIndex, FirstElement) {
  EXPECT_EQ((GetIndexV<TList, int, TList<int, double, char>>), 0u);
}

TEST(HeterogeneousGetIndex, MiddleElement) {
  EXPECT_EQ((GetIndexV<TList, double, TList<int, double, char>>), 1u);
}

TEST(HeterogeneousGetIndex, LastElement) {
  EXPECT_EQ((GetIndexV<TList, char, TList<int, double, char>>), 2u);
}

// --- GetDestinationPos ---

TEST(HeterogeneousDestinationPos, SingleList) {
  using L1 = TList<int, double, char>;
  constexpr auto pos0 = GetDestinationPosV<TList, 0, L1>;
  EXPECT_EQ(pos0.list_pos, 0u);
  EXPECT_EQ(pos0.elem_pos, 0u);

  constexpr auto pos2 = GetDestinationPosV<TList, 2, L1>;
  EXPECT_EQ(pos2.list_pos, 0u);
  EXPECT_EQ(pos2.elem_pos, 2u);
}

TEST(HeterogeneousDestinationPos, MultipleLists) {
  using L1 = TList<int, double>;
  using L2 = TList<char>;
  using L3 = TList<float, long>;

  constexpr auto pos0 = GetDestinationPosV<TList, 0, L1, L2, L3>;
  EXPECT_EQ(pos0.list_pos, 0u);
  EXPECT_EQ(pos0.elem_pos, 0u);

  constexpr auto pos1 = GetDestinationPosV<TList, 1, L1, L2, L3>;
  EXPECT_EQ(pos1.list_pos, 0u);
  EXPECT_EQ(pos1.elem_pos, 1u);

  constexpr auto pos2 = GetDestinationPosV<TList, 2, L1, L2, L3>;
  EXPECT_EQ(pos2.list_pos, 1u);
  EXPECT_EQ(pos2.elem_pos, 0u);

  constexpr auto pos3 = GetDestinationPosV<TList, 3, L1, L2, L3>;
  EXPECT_EQ(pos3.list_pos, 2u);
  EXPECT_EQ(pos3.elem_pos, 0u);

  constexpr auto pos4 = GetDestinationPosV<TList, 4, L1, L2, L3>;
  EXPECT_EQ(pos4.list_pos, 2u);
  EXPECT_EQ(pos4.elem_pos, 1u);
}

// --- CatLists ---

TEST(HeterogeneousCatLists, CatTwoNonEmpty) {
  using Result = CatListsT<TList, TList<int, double>, TList<char, float>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<int, double, char, float>>));
}

TEST(HeterogeneousCatLists, CatWithEmpty) {
  using Result = CatListsT<TList, TList<int>, TList<>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<int>>));
}

TEST(HeterogeneousCatLists, CatEmptyWithNonEmpty) {
  using Result = CatListsT<TList, TList<>, TList<int>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<int>>));
}

TEST(HeterogeneousCatLists, CatThreeLists) {
  using Result = CatListsT<TList, TList<int>, TList<double>, TList<char>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<int, double, char>>));
}

TEST(HeterogeneousCatLists, CatSingleList) {
  using Result = CatListsT<TList, TList<int, double>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<int, double>>));
}

TEST(HeterogeneousCatLists, CatAllEmpty) {
  using Result = CatListsT<TList, TList<>, TList<>, TList<>>;
  EXPECT_TRUE((std::is_same_v<Result, TList<>>));
}

}  // namespace