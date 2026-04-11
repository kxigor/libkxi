#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <string>
#include <type_traits>

namespace {

using kxi::tuple::FlatTuple;
using kxi::tuple::TupleElementT;
using kxi::tuple::TupleSizeV;

// --- TupleSize ---

TEST(TupleCore, SizeEmpty) { EXPECT_EQ(TupleSizeV<FlatTuple<>>, 0u); }

TEST(TupleCore, SizeSingle) { EXPECT_EQ(TupleSizeV<FlatTuple<int>>, 1u); }

TEST(TupleCore, SizeMultiple) {
  EXPECT_EQ((TupleSizeV<FlatTuple<int, double, char>>), 3u);
}

// --- TupleElement ---

TEST(TupleCore, ElementTypes) {
  using T = FlatTuple<int, double, std::string>;
  EXPECT_TRUE((std::is_same_v<TupleElementT<0, T>, int>));
  EXPECT_TRUE((std::is_same_v<TupleElementT<1, T>, double>));
  EXPECT_TRUE((std::is_same_v<TupleElementT<2, T>, std::string>));
}

// --- Default construction ---

TEST(TupleCore, DefaultConstruct) {
  FlatTuple<int, double> t;
  EXPECT_EQ(t.template Get<0>(), 0);
  EXPECT_EQ(t.template Get<1>(), 0.0);
}

// --- Value construction ---

TEST(TupleCore, ValueConstruct) {
  FlatTuple<int, double, std::string> t(42, 3.14, "hello");
  EXPECT_EQ(t.template Get<0>(), 42);
  EXPECT_DOUBLE_EQ(t.template Get<1>(), 3.14);
  EXPECT_EQ(t.template Get<2>(), "hello");
}

// --- Copy construction ---

TEST(TupleCore, CopyConstruct) {
  FlatTuple<int, std::string> t1(10, "abc");
  FlatTuple<int, std::string> t2(t1);
  EXPECT_EQ(t2.template Get<0>(), 10);
  EXPECT_EQ(t2.template Get<1>(), "abc");
  // original unmodified
  EXPECT_EQ(t1.template Get<0>(), 10);
  EXPECT_EQ(t1.template Get<1>(), "abc");
}

// --- Move construction ---

TEST(TupleCore, MoveConstruct) {
  FlatTuple<int, std::string> t1(10, "abc");
  FlatTuple<int, std::string> t2(std::move(t1));
  EXPECT_EQ(t2.template Get<0>(), 10);
  EXPECT_EQ(t2.template Get<1>(), "abc");
}

// --- Copy assignment ---

TEST(TupleCore, CopyAssign) {
  FlatTuple<int, std::string> t1(10, "abc");
  FlatTuple<int, std::string> t2;
  t2 = t1;
  EXPECT_EQ(t2.template Get<0>(), 10);
  EXPECT_EQ(t2.template Get<1>(), "abc");
}

// --- Move assignment ---

TEST(TupleCore, MoveAssign) {
  FlatTuple<int, std::string> t1(10, "abc");
  FlatTuple<int, std::string> t2;
  t2 = std::move(t1);
  EXPECT_EQ(t2.template Get<0>(), 10);
  EXPECT_EQ(t2.template Get<1>(), "abc");
}

// --- Get by index ---

TEST(TupleCore, GetByIndexMutable) {
  FlatTuple<int, double> t(1, 2.0);
  t.template Get<0>() = 42;
  EXPECT_EQ(t.template Get<0>(), 42);
}

TEST(TupleCore, GetByIndexConst) {
  const FlatTuple<int, double> t(1, 2.0);
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_TRUE((std::is_same_v<decltype(t.template Get<0>()), const int&>));
}

// --- Get by type ---

TEST(TupleCore, GetByType) {
  FlatTuple<int, double, std::string> t(1, 2.0, "x");
  EXPECT_EQ(t.template Get<int>(), 1);
  EXPECT_DOUBLE_EQ(t.template Get<double>(), 2.0);
  EXPECT_EQ(t.template Get<std::string>(), "x");
}

TEST(TupleCore, GetByTypeMutable) {
  FlatTuple<int, double> t(1, 2.0);
  t.template Get<int>() = 99;
  EXPECT_EQ(t.template Get<int>(), 99);
}

// --- Swap ---

TEST(TupleCore, Swap) {
  FlatTuple<int, std::string> t1(1, "a");
  FlatTuple<int, std::string> t2(2, "b");
  t1.Swap(t2);
  EXPECT_EQ(t1.template Get<0>(), 2);
  EXPECT_EQ(t1.template Get<1>(), "b");
  EXPECT_EQ(t2.template Get<0>(), 1);
  EXPECT_EQ(t2.template Get<1>(), "a");
}

}  // namespace