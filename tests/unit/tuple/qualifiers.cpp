#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::tuple::Tuple;

// Helper: check that Get returns the expected qualified type
// For member Get with explicit deducing this:
//   lvalue       -> T&
//   const lvalue -> const T&
//   rvalue       -> T&&
//   const rvalue -> const T&&

// --- Member Get by index ---

TEST(TupleQualifiers, GetByIndex_Lvalue) {
  Tuple<int, std::string> t(1, "hello");
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), int&>));
  v = 42;
  EXPECT_EQ(t.template Get<0>(), 42);
}

TEST(TupleQualifiers, GetByIndex_ConstLvalue) {
  const Tuple<int, std::string> t(1, "hello");
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&>));
  EXPECT_EQ(v, 1);
}

TEST(TupleQualifiers, GetByIndex_Rvalue) {
  Tuple<int, std::string> t(1, "hello");
  decltype(auto) v = std::move(t).template Get<1>();
  EXPECT_TRUE((std::is_same_v<decltype(v), std::string&&>));
}

TEST(TupleQualifiers, GetByIndex_ConstRvalue) {
  const Tuple<int, std::string> t(1, "hello");
  decltype(auto) v = std::move(t).template Get<1>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const std::string&&>));
}

// --- Member Get by type ---

TEST(TupleQualifiers, GetByType_Lvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = t.template Get<int>();
  EXPECT_TRUE((std::is_same_v<decltype(v), int&>));
}

TEST(TupleQualifiers, GetByType_ConstLvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = t.template Get<int>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&>));
}

TEST(TupleQualifiers, GetByType_Rvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = std::move(t).template Get<int>();
  EXPECT_TRUE((std::is_same_v<decltype(v), int&&>));
}

TEST(TupleQualifiers, GetByType_ConstRvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = std::move(t).template Get<int>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&&>));
}

// --- Free Get by index ---

TEST(TupleQualifiers, FreeGetByIndex_Lvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<0>(t);
  EXPECT_TRUE((std::is_same_v<decltype(v), int&>));
}

TEST(TupleQualifiers, FreeGetByIndex_ConstLvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<0>(t);
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&>));
}

TEST(TupleQualifiers, FreeGetByIndex_Rvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<0>(std::move(t));
  EXPECT_TRUE((std::is_same_v<decltype(v), int&&>));
}

TEST(TupleQualifiers, FreeGetByIndex_ConstRvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<0>(std::move(t));
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&&>));
}

// --- Free Get by type ---

TEST(TupleQualifiers, FreeGetByType_Lvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<int>(t);
  EXPECT_TRUE((std::is_same_v<decltype(v), int&>));
}

TEST(TupleQualifiers, FreeGetByType_ConstLvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<int>(t);
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&>));
}

TEST(TupleQualifiers, FreeGetByType_Rvalue) {
  Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<int>(std::move(t));
  EXPECT_TRUE((std::is_same_v<decltype(v), int&&>));
}

TEST(TupleQualifiers, FreeGetByType_ConstRvalue) {
  const Tuple<int, double> t(1, 2.0);
  decltype(auto) v = kxi::tuple::Get<int>(std::move(t));
  EXPECT_TRUE((std::is_same_v<decltype(v), const int&&>));
}

// --- Tuple holding references ---

TEST(TupleQualifiers, HoldsLvalueRef) {
  int x = 10;
  Tuple<int&> t(x);
  t.template Get<0>() = 42;
  EXPECT_EQ(x, 42);
}

TEST(TupleQualifiers, HoldsConstLvalueRef) {
  const int x = 10;
  Tuple<const int&> t(x);
  EXPECT_EQ(t.template Get<0>(), 10);
  EXPECT_TRUE((std::is_same_v<decltype(t.template Get<0>()), const int&>));
}

// --- Volatile qualifier ---

TEST(TupleQualifiers, GetByIndex_VolatileLvalue) {
  volatile Tuple<int, double> t(1, 2.0);
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), volatile int&>));
}

TEST(TupleQualifiers, GetByIndex_ConstVolatileLvalue) {
  const volatile Tuple<int, double> t(1, 2.0);
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const volatile int&>));
}

}  // namespace