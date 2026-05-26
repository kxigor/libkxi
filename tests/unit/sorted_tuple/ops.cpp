#include <gtest/gtest.h>

#include <libkxi/tuple/like.hpp>
#include <libkxi/tuple/sorted.hpp>
#include <type_traits>
#include <utility>

namespace {

using kxi::tuple::sorted::Of;

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr bool value = sizeof(LHS) < sizeof(RHS);
};

// =============================================================
// Factories — sorted::Make / Tie / Forward / ForwardInto
//
// Predicate is supplied explicitly as a template-template argument; the
// element types are deduced from the call arguments.
// =============================================================

TEST(SortedTupleOps, MakeBasic) {
  auto t = kxi::tuple::sorted::Make<SizeofLess>(1, 2.0, 'x');
  EXPECT_TRUE((std::is_same_v<decltype(t),
                              Of<SizeofLess>::Tuple<int, double, char>>));
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(t.template Get<1>(), 2.0);
  EXPECT_EQ(t.template Get<2>(), 'x');
}

TEST(SortedTupleOps, MakeUnwrapsRefWrapper) {
  int x = 42;
  auto t = kxi::tuple::sorted::Make<SizeofLess>(std::ref(x));
  EXPECT_TRUE((std::is_same_v<decltype(t), Of<SizeofLess>::Tuple<int&>>));
  t.template Get<0>() = 99;
  EXPECT_EQ(x, 99);
}

TEST(SortedTupleOps, TieHoldsLvalueReferences) {
  int a = 1;
  double b = 2.0;
  auto t = kxi::tuple::sorted::Tie<SizeofLess>(a, b);
  EXPECT_TRUE(
      (std::is_same_v<decltype(t), Of<SizeofLess>::Tuple<int&, double&>>));
  t.template Get<0>() = 10;
  EXPECT_EQ(a, 10);
}

TEST(SortedTupleOps, ForwardLvalues) {
  int x = 1;
  double y = 2.0;
  auto t = kxi::tuple::sorted::Forward<SizeofLess>(x, y);
  EXPECT_TRUE(
      (std::is_same_v<decltype(t), Of<SizeofLess>::Tuple<int&, double&>>));
}

TEST(SortedTupleOps, ForwardRvalues) {
  auto t = kxi::tuple::sorted::Forward<SizeofLess>(1, 2.0);
  EXPECT_TRUE(
      (std::is_same_v<decltype(t), Of<SizeofLess>::Tuple<int&&, double&&>>));
}

TEST(SortedTupleOps, ForwardIntoPreservesArgTypes) {
  int x = 1;
  auto t = kxi::tuple::sorted::ForwardInto<SizeofLess>(x, 2.0);
  EXPECT_TRUE(
      (std::is_same_v<decltype(t), Of<SizeofLess>::Tuple<int&, double>>));
}

// =============================================================
// Free like:: API must accept Of<Pred>::Tuple
// =============================================================

TEST(SortedTupleOps, FreeLikeGetByIndex) {
  Of<SizeofLess>::Tuple<int, double, char> t(1, 2.0, 'x');
  EXPECT_EQ((kxi::tuple::like::Get<0>(t)), 1);
  EXPECT_DOUBLE_EQ((kxi::tuple::like::Get<1>(t)), 2.0);
  EXPECT_EQ((kxi::tuple::like::Get<2>(t)), 'x');
}

TEST(SortedTupleOps, FreeLikeGetByType) {
  Of<SizeofLess>::Tuple<int, double, char> t(1, 2.0, 'x');
  EXPECT_EQ((kxi::tuple::like::Get<int>(t)), 1);
  EXPECT_DOUBLE_EQ((kxi::tuple::like::Get<double>(t)), 2.0);
  EXPECT_EQ((kxi::tuple::like::Get<char>(t)), 'x');
}

TEST(SortedTupleOps, FreeLikeGetForwardsCategory) {
  Of<SizeofLess>::Tuple<int, double> t(1, 2.0);
  decltype(auto) r = kxi::tuple::like::Get<0>(std::move(t));
  EXPECT_TRUE((std::is_same_v<decltype(r), int&&>));
}

TEST(SortedTupleOps, FreeLikeSwap) {
  Of<SizeofLess>::Tuple<int, double> a(1, 2.0);
  Of<SizeofLess>::Tuple<int, double> b(10, 20.0);
  kxi::tuple::like::Swap(a, b);
  EXPECT_EQ(a.template Get<0>(), 10);
  EXPECT_DOUBLE_EQ(a.template Get<1>(), 20.0);
  EXPECT_EQ(b.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(b.template Get<1>(), 2.0);
}

// =============================================================
// sorted:: re-exports of like:: must be visible at the right namespace
// =============================================================

TEST(SortedTupleOps, NamespaceReexportsGet) {
  Of<SizeofLess>::Tuple<int, double> t(1, 2.0);
  EXPECT_EQ((kxi::tuple::sorted::Get<0>(t)), 1);
  EXPECT_DOUBLE_EQ((kxi::tuple::sorted::Get<double>(t)), 2.0);
}

TEST(SortedTupleOps, NamespaceReexportsSwap) {
  Of<SizeofLess>::Tuple<int, double> a(1, 2.0);
  Of<SizeofLess>::Tuple<int, double> b(10, 20.0);
  kxi::tuple::sorted::Swap(a, b);
  EXPECT_EQ(a.template Get<0>(), 10);
  EXPECT_EQ(b.template Get<0>(), 1);
}

}  // namespace
