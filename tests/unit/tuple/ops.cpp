#include <gtest/gtest.h>

#include <functional>
#include <libkxi/tuple/flat.hpp>
#include <libkxi/tuple/like.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::tuple::flat::Cat;
using kxi::tuple::flat::FlatTuple;
using kxi::tuple::flat::Forward;
using kxi::tuple::flat::Make;
using kxi::tuple::flat::Tie;
using kxi::tuple::like::PackGet;
using kxi::tuple::like::Swap;

// --- Make ---

TEST(TupleOps, MakeTupleBasic) {
  auto t = Make(1, 2.0, std::string("hi"));
  EXPECT_TRUE(
      (std::is_same_v<decltype(t), FlatTuple<int, double, std::string>>));
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(t.template Get<1>(), 2.0);
  EXPECT_EQ(t.template Get<2>(), "hi");
}

TEST(TupleOps, MakeTupleUnwrapsRefWrapper) {
  int x = 42;
  auto t = Make(std::ref(x));
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&>>));
  t.template Get<0>() = 99;
  EXPECT_EQ(x, 99);
}

TEST(TupleOps, MakeTupleDecaysArrayAndFunction) {
  const char arr[] = "hello";
  auto t = Make(arr);
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

// --- Forward ---

TEST(TupleOps, ForwardAsTupleLvalues) {
  int x = 1;
  double y = 2.0;
  auto t = Forward(x, y);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&, double&>>));
  t.template Get<0>() = 42;
  EXPECT_EQ(x, 42);
}

TEST(TupleOps, ForwardAsTupleRvalues) {
  auto t = Forward(1, 2.0);
  EXPECT_TRUE((std::is_same_v<decltype(t), FlatTuple<int&&, double&&>>));
}

TEST(TupleOps, ForwardAsTupleMixed) {
  int x = 1;
  auto t = Forward(x, 2.0);
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

// --- PackGet ---

TEST(TupleOps, FlatGetFromSingleTuple) {
  FlatTuple<int, double> t(10, 3.14);
  EXPECT_EQ((PackGet<0>(t)), 10);
  EXPECT_DOUBLE_EQ((PackGet<1>(t)), 3.14);
}

TEST(TupleOps, FlatGetFromMultipleTuples) {
  FlatTuple<int, double> t1(1, 2.0);
  FlatTuple<std::string, char> t2("hello", 'x');

  // flat indices: 0=int, 1=double, 2=string, 3=char
  EXPECT_EQ((PackGet<0>(t1, t2)), 1);
  EXPECT_DOUBLE_EQ((PackGet<1>(t1, t2)), 2.0);
  EXPECT_EQ((PackGet<2>(t1, t2)), "hello");
  EXPECT_EQ((PackGet<3>(t1, t2)), 'x');
}

TEST(TupleOps, FlatGetMutates) {
  FlatTuple<int> t1(1);
  FlatTuple<double> t2(2.0);
  PackGet<0>(t1, t2) = 99;
  EXPECT_EQ(t1.template Get<0>(), 99);
}

// --- Cat ---

TEST(TupleOps, TupleCatTwoTuples) {
  FlatTuple<int, double> t1(1, 2.0);
  FlatTuple<std::string> t2("hi");
  auto result = Cat(t1, t2);
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
  auto result = Cat(t1, t2, t3);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int, double, char>>));
  EXPECT_EQ(result.template Get<0>(), 1);
  EXPECT_DOUBLE_EQ(result.template Get<1>(), 2.0);
  EXPECT_EQ(result.template Get<2>(), 'c');
}

TEST(TupleOps, TupleCatWithEmpty) {
  FlatTuple<int> t1(1);
  FlatTuple<> t2;
  auto result = Cat(t1, t2);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

TEST(TupleOps, TupleCatSingleTuple) {
  FlatTuple<int, double> t(1, 2.0);
  auto result = Cat(t);
  EXPECT_TRUE((std::is_same_v<decltype(result), FlatTuple<int, double>>));
  EXPECT_EQ(result.template Get<0>(), 1);
}

}  // namespace