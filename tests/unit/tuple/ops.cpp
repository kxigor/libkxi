#include <gtest/gtest.h>

#include <functional>
#include <libkxi/tuple.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::tuple::FlatGet;
using kxi::tuple::ForwardAsTuple;
using kxi::tuple::MakeTuple;
using kxi::tuple::Swap;
using kxi::tuple::Tie;
using kxi::tuple::FlatTuple;
using kxi::tuple::TupleCat;

// --- MakeTuple ---

TEST(TupleOps, MakeTupleBasic) {
  auto t = MakeTuple(1, 2.0, std::string("hi"));
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int, double, std::string>>));
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(t.template Get<1>(), 2.0);
  EXPECT_EQ(t.template Get<2>(), "hi");
}

TEST(TupleOps, MakeTupleUnwrapsRefWrapper) {
  int x = 42;
  auto t = MakeTuple(std::ref(x));
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&>>));
  t.template Get<0>() = 99;
  EXPECT_EQ(x, 99);
}

TEST(TupleOps, MakeTupleDecaysArrayAndFunction) {
  const char arr[] = "hello";
  auto t = MakeTuple(arr);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<const char*>>));
}

// --- Tie ---

TEST(TupleOps, TieBasic) {
  int a = 1;
  double b = 2.0;
  auto t = Tie(a, b);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&, double&>>));
  t.template Get<0>() = 10;
  EXPECT_EQ(a, 10);
}

// --- ForwardAsTuple ---

TEST(TupleOps, ForwardAsTupleLvalues) {
  int x = 1;
  double y = 2.0;
  auto t = ForwardAsTuple(x, y);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&, double&>>));
  t.template Get<0>() = 42;
  EXPECT_EQ(x, 42);
}

TEST(TupleOps, ForwardAsTupleRvalues) {
  auto t = ForwardAsTuple(1, 2.0);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&&, double&&>>));
}

TEST(TupleOps, ForwardAsTupleMixed) {
  int x = 1;
  auto t = ForwardAsTuple(x, 2.0);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&, double&&>>));
}

// --- Swap (free function) ---

TEST(TupleOps, SwapFree) {
  FlatTuple<int, std::string> t1(1, "a");
  FlatTuple<int, std::string> t2(2, "b");
  Swap(t1, t2);
  EXPECT_EQ(t1.template Get<0>(), 2);
  EXPECT_EQ(t1.template Get<1>(), "b");
  EXPECT_EQ(t2.template Get<0>(), 1);
  EXPECT_EQ(t2.template Get<1>(), "a");
}

// --- FlatGet ---

TEST(TupleOps, FlatGetFromSingleTuple) {
  FlatTuple<int, double> t(10, 3.14);
  EXPECT_EQ((FlatGet<0>(t)), 10);
  EXPECT_DOUBLE_EQ((FlatGet<1>(t)), 3.14);
}

TEST(TupleOps, FlatGetFromMultipleTuples) {
  FlatTuple<int, double> t1(1, 2.0);
  FlatTuple<std::string, char> t2("hello", 'x');

  // flat indices: 0=int, 1=double, 2=string, 3=char
  EXPECT_EQ((FlatGet<0>(t1, t2)), 1);
  EXPECT_DOUBLE_EQ((FlatGet<1>(t1, t2)), 2.0);
  EXPECT_EQ((FlatGet<2>(t1, t2)), "hello");
  EXPECT_EQ((FlatGet<3>(t1, t2)), 'x');
}

TEST(TupleOps, FlatGetMutates) {
  FlatTuple<int> t1(1);
  FlatTuple<double> t2(2.0);
  FlatGet<0>(t1, t2) = 99;
  EXPECT_EQ(t1.template Get<0>(), 99);
}

// --- TupleCat ---

TEST(TupleOps, TupleCatTwoTuples) {
  FlatTuple<int, double> t1(1, 2.0);
  FlatTuple<std::string> t2("hi");
  auto result = TupleCat(t1, t2);
  EXPECT_TRUE(
      (std::is_same_v<decltype(result), FlatTuple<int, double, std::string>>));
  EXPECT_EQ(result.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(result.template Get<1>(), 2.0);
  EXPECT_EQ(result.template Get<2>(), "hi");
}

TEST(TupleOps, TupleCatThreeTuples) {
  FlatTuple<int> t1(1);
  FlatTuple<double> t2(2.0);
  FlatTuple<char> t3('c');
  auto result = TupleCat(t1, t2, t3);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int, double, char>>));
  EXPECT_EQ(result.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(result.template Get<1>(), 2.0);
  EXPECT_EQ(result.template Get<2>(), 'c');
}

TEST(TupleOps, TupleCatWithEmpty) {
  FlatTuple<int> t1(1);
  FlatTuple<> t2;
  auto result = TupleCat(t1, t2);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

TEST(TupleOps, TupleCatSingleTuple) {
  FlatTuple<int, double> t(1, 2.0);
  auto result = TupleCat(t);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int, double>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

}  // namespace