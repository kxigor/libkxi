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
using kxi::tuple::Tuple;
using kxi::tuple::TupleCat;

// --- MakeTuple ---

TEST(TupleOps, MakeTupleBasic) {
  auto t = MakeTuple(1, 2.0, std::string("hi"));
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int, double, std::string>>));
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(t.template Get<1>(), 2.0);
  EXPECT_EQ(t.template Get<2>(), "hi");
}

TEST(TupleOps, MakeTupleUnwrapsRefWrapper) {
  int x = 42;
  auto t = MakeTuple(std::ref(x));
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int&>>));
  t.template Get<0>() = 99;
  EXPECT_EQ(x, 99);
}

TEST(TupleOps, MakeTupleDecaysArrayAndFunction) {
  const char arr[] = "hello";
  auto t = MakeTuple(arr);
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<const char*>>));
}

// --- Tie ---

TEST(TupleOps, TieBasic) {
  int a = 1;
  double b = 2.0;
  auto t = Tie(a, b);
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int&, double&>>));
  t.template Get<0>() = 10;
  EXPECT_EQ(a, 10);
}

// --- ForwardAsTuple ---

TEST(TupleOps, ForwardAsTupleLvalues) {
  int x = 1;
  double y = 2.0;
  auto t = ForwardAsTuple(x, y);
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int&, double&>>));
  t.template Get<0>() = 42;
  EXPECT_EQ(x, 42);
}

TEST(TupleOps, ForwardAsTupleRvalues) {
  auto t = ForwardAsTuple(1, 2.0);
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int&&, double&&>>));
}

TEST(TupleOps, ForwardAsTupleMixed) {
  int x = 1;
  auto t = ForwardAsTuple(x, 2.0);
  EXPECT_TRUE((std::is_same_v<decltype(t), Tuple<int&, double&&>>));
}

// --- Swap (free function) ---

TEST(TupleOps, SwapFree) {
  Tuple<int, std::string> t1(1, "a");
  Tuple<int, std::string> t2(2, "b");
  Swap(t1, t2);
  EXPECT_EQ(t1.template Get<0>(), 2);
  EXPECT_EQ(t1.template Get<1>(), "b");
  EXPECT_EQ(t2.template Get<0>(), 1);
  EXPECT_EQ(t2.template Get<1>(), "a");
}

// --- FlatGet ---

TEST(TupleOps, FlatGetFromSingleTuple) {
  Tuple<int, double> t(10, 3.14);
  EXPECT_EQ((FlatGet<0>(t)), 10);
  EXPECT_DOUBLE_EQ((FlatGet<1>(t)), 3.14);
}

TEST(TupleOps, FlatGetFromMultipleTuples) {
  Tuple<int, double> t1(1, 2.0);
  Tuple<std::string, char> t2("hello", 'x');

  // flat indices: 0=int, 1=double, 2=string, 3=char
  EXPECT_EQ((FlatGet<0>(t1, t2)), 1);
  EXPECT_DOUBLE_EQ((FlatGet<1>(t1, t2)), 2.0);
  EXPECT_EQ((FlatGet<2>(t1, t2)), "hello");
  EXPECT_EQ((FlatGet<3>(t1, t2)), 'x');
}

TEST(TupleOps, FlatGetMutates) {
  Tuple<int> t1(1);
  Tuple<double> t2(2.0);
  FlatGet<0>(t1, t2) = 99;
  EXPECT_EQ(t1.template Get<0>(), 99);
}

// --- TupleCat ---

TEST(TupleOps, TupleCatTwoTuples) {
  Tuple<int, double> t1(1, 2.0);
  Tuple<std::string> t2("hi");
  auto result = TupleCat(t1, t2);
  EXPECT_TRUE(
      (std::is_same_v<decltype(result), Tuple<int, double, std::string>>));
  EXPECT_EQ(result.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(result.template Get<1>(), 2.0);
  EXPECT_EQ(result.template Get<2>(), "hi");
}

TEST(TupleOps, TupleCatThreeTuples) {
  Tuple<int> t1(1);
  Tuple<double> t2(2.0);
  Tuple<char> t3('c');
  auto result = TupleCat(t1, t2, t3);
  EXPECT_TRUE((std::is_same_v<decltype(result), Tuple<int, double, char>>));
  EXPECT_EQ(result.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(result.template Get<1>(), 2.0);
  EXPECT_EQ(result.template Get<2>(), 'c');
}

TEST(TupleOps, TupleCatWithEmpty) {
  Tuple<int> t1(1);
  Tuple<> t2;
  auto result = TupleCat(t1, t2);
  EXPECT_TRUE((std::is_same_v<decltype(result), Tuple<int>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

TEST(TupleOps, TupleCatSingleTuple) {
  Tuple<int, double> t(1, 2.0);
  auto result = TupleCat(t);
  EXPECT_TRUE((std::is_same_v<decltype(result), Tuple<int, double>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

}  // namespace