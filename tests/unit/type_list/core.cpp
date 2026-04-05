#include <gtest/gtest.h>
#include <libkxi/type_list.hpp>
#include <type_traits>

namespace {

using kxi::type_list::CatListsT;
using kxi::type_list::GetCount;
using kxi::type_list::GetDestinationPosV;
using kxi::type_list::GetIndexV;
using kxi::type_list::GetSizeV;
using kxi::type_list::GetTypeT;
using kxi::type_list::TypeList;

// --- GetSize ---

TEST(TypeListGetSize, EmptyList) {
  EXPECT_EQ((GetSizeV<TypeList<>>), 0u);
}

TEST(TypeListGetSize, SingleElement) {
  EXPECT_EQ((GetSizeV<TypeList<int>>), 1u);
}

TEST(TypeListGetSize, MultipleElements) {
  EXPECT_EQ((GetSizeV<TypeList<int, double, char>>), 3u);
}

// --- GetType ---

TEST(TypeListGetType, FirstElement) {
  EXPECT_TRUE((std::is_same_v<GetTypeT<0, TypeList<int, double, char>>, int>));
}

TEST(TypeListGetType, MiddleElement) {
  EXPECT_TRUE((std::is_same_v<GetTypeT<1, TypeList<int, double, char>>, double>));
}

TEST(TypeListGetType, LastElement) {
  EXPECT_TRUE((std::is_same_v<GetTypeT<2, TypeList<int, double, char>>, char>));
}

TEST(TypeListGetType, SingleElementList) {
  EXPECT_TRUE((std::is_same_v<GetTypeT<0, TypeList<float>>, float>));
}

// --- GetCount ---

TEST(TypeListGetCount, TypeNotPresent) {
  EXPECT_EQ((GetCount<float, TypeList<int, double, char>>::value), 0u);
}

TEST(TypeListGetCount, TypePresentOnce) {
  EXPECT_EQ((GetCount<int, TypeList<int, double, char>>::value), 1u);
}

TEST(TypeListGetCount, TypePresentMultipleTimes) {
  EXPECT_EQ((GetCount<int, TypeList<int, double, int, char, int>>::value), 3u);
}

TEST(TypeListGetCount, EmptyList) {
  EXPECT_EQ((GetCount<int, TypeList<>>::value), 0u);
}

// --- GetIndex ---

TEST(TypeListGetIndex, FirstElement) {
  EXPECT_EQ((GetIndexV<int, TypeList<int, double, char>>), 0u);
}

TEST(TypeListGetIndex, MiddleElement) {
  EXPECT_EQ((GetIndexV<double, TypeList<int, double, char>>), 1u);
}

TEST(TypeListGetIndex, LastElement) {
  EXPECT_EQ((GetIndexV<char, TypeList<int, double, char>>), 2u);
}

// --- GetDestinationPos ---

TEST(TypeListDestinationPos, SingleList) {
  using L1 = TypeList<int, double, char>;
  constexpr auto pos0 = GetDestinationPosV<0, L1>;
  EXPECT_EQ(pos0.list_pos, 0u);
  EXPECT_EQ(pos0.elem_pos, 0u);

  constexpr auto pos2 = GetDestinationPosV<2, L1>;
  EXPECT_EQ(pos2.list_pos, 0u);
  EXPECT_EQ(pos2.elem_pos, 2u);
}

TEST(TypeListDestinationPos, MultipleLists) {
  using L1 = TypeList<int, double>;
  using L2 = TypeList<char>;
  using L3 = TypeList<float, long>;

  // Index 0 -> list 0, elem 0
  constexpr auto pos0 = GetDestinationPosV<0, L1, L2, L3>;
  EXPECT_EQ(pos0.list_pos, 0u);
  EXPECT_EQ(pos0.elem_pos, 0u);

  // Index 1 -> list 0, elem 1
  constexpr auto pos1 = GetDestinationPosV<1, L1, L2, L3>;
  EXPECT_EQ(pos1.list_pos, 0u);
  EXPECT_EQ(pos1.elem_pos, 1u);

  // Index 2 -> list 1, elem 0
  constexpr auto pos2 = GetDestinationPosV<2, L1, L2, L3>;
  EXPECT_EQ(pos2.list_pos, 1u);
  EXPECT_EQ(pos2.elem_pos, 0u);

  // Index 3 -> list 2, elem 0
  constexpr auto pos3 = GetDestinationPosV<3, L1, L2, L3>;
  EXPECT_EQ(pos3.list_pos, 2u);
  EXPECT_EQ(pos3.elem_pos, 0u);

  // Index 4 -> list 2, elem 1
  constexpr auto pos4 = GetDestinationPosV<4, L1, L2, L3>;
  EXPECT_EQ(pos4.list_pos, 2u);
  EXPECT_EQ(pos4.elem_pos, 1u);
}

// --- CatLists ---

TEST(TypeListCatLists, CatTwoNonEmpty) {
  using Result = CatListsT<TypeList<int, double>, TypeList<char, float>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double, char, float>>));
}

TEST(TypeListCatLists, CatWithEmpty) {
  using Result = CatListsT<TypeList<int>, TypeList<>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int>>));
}

TEST(TypeListCatLists, CatEmptyWithNonEmpty) {
  using Result = CatListsT<TypeList<>, TypeList<int>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int>>));
}

TEST(TypeListCatLists, CatThreeLists) {
  using Result = CatListsT<TypeList<int>, TypeList<double>, TypeList<char>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double, char>>));
}

TEST(TypeListCatLists, CatSingleList) {
  using Result = CatListsT<TypeList<int, double>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListCatLists, CatAllEmpty) {
  using Result = CatListsT<TypeList<>, TypeList<>, TypeList<>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<>>));
}

}  // namespace
