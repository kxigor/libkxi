#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/sorted_tuple.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::sorted_tuple::SortedTuple;

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

template <typename LHS, typename RHS>
struct SizeofGreater {
  static constexpr const bool value = sizeof(LHS) > sizeof(RHS);
};

// --- Default construction ---

TEST(SortedTupleCore, DefaultConstruct) {
  SortedTuple<SizeofLess, int, double> t;
  EXPECT_EQ(t.template Get<0>(), 0);
  EXPECT_EQ(t.template Get<1>(), 0.0);
}

// --- Value construction: elements reordered internally but Get uses original
// index ---

TEST(SortedTupleCore, ValueConstructAndGetByOriginalIndex) {
  // Original order: double(8), char(1), int(4)
  // Sorted internally: char, int, double
  // But Get<0> should return the double (first original arg),
  // Get<1> the char, Get<2> the int
  SortedTuple<SizeofLess, double, char, int> t(3.14, 'x', 42);
  EXPECT_DOUBLE_EQ(t.template Get<0>(), 3.14);
  EXPECT_EQ(t.template Get<1>(), 'x');
  EXPECT_EQ(t.template Get<2>(), 42);
}

TEST(SortedTupleCore, ValueConstructAlreadySorted) {
  // char(1), int(4), double(8) — already sorted ascending
  SortedTuple<SizeofLess, char, int, double> t('a', 10, 2.5);
  EXPECT_EQ(t.template Get<0>(), 'a');
  EXPECT_EQ(t.template Get<1>(), 10);
  EXPECT_DOUBLE_EQ(t.template Get<2>(), 2.5);
}

TEST(SortedTupleCore, ValueConstructReverseSorted) {
  // double(8), int(4), char(1) — reverse of ascending
  SortedTuple<SizeofLess, double, int, char> t(1.5, 10, 'z');
  EXPECT_DOUBLE_EQ(t.template Get<0>(), 1.5);
  EXPECT_EQ(t.template Get<1>(), 10);
  EXPECT_EQ(t.template Get<2>(), 'z');
}

TEST(SortedTupleCore, ValueConstructWithGreaterPredicate) {
  // Descending sort: double first, then int, then char
  SortedTuple<SizeofGreater, char, int, double> t('a', 10, 2.5);
  EXPECT_EQ(t.template Get<0>(), 'a');
  EXPECT_EQ(t.template Get<1>(), 10);
  EXPECT_DOUBLE_EQ(t.template Get<2>(), 2.5);
}

TEST(SortedTupleCore, SingleElement) {
  SortedTuple<SizeofLess, int> t(42);
  EXPECT_EQ(t.template Get<0>(), 42);
}

TEST(SortedTupleCore, FiveElements) {
  // Original: double(8), int32(4), int8(1), float(4), int16(2)
  SortedTuple<SizeofLess, double, std::int32_t, std::int8_t, float,
              std::int16_t>
      t(1.0, 2, std::int8_t{3}, 4.0f, std::int16_t{5});
  EXPECT_DOUBLE_EQ(t.template Get<0>(), 1.0);
  EXPECT_EQ(t.template Get<1>(), 2);
  EXPECT_EQ(t.template Get<2>(), std::int8_t{3});
  EXPECT_FLOAT_EQ(t.template Get<3>(), 4.0f);
  EXPECT_EQ(t.template Get<4>(), std::int16_t{5});
}

// --- Get by type ---

TEST(SortedTupleCore, GetByType) {
  SortedTuple<SizeofLess, double, char, int> t(3.14, 'x', 42);
  EXPECT_DOUBLE_EQ(t.template Get<double>(), 3.14);
  EXPECT_EQ(t.template Get<char>(), 'x');
  EXPECT_EQ(t.template Get<int>(), 42);
}

// --- Copy construction ---

TEST(SortedTupleCore, CopyConstruct) {
  SortedTuple<SizeofLess, double, char, int> t1(3.14, 'x', 42);
  SortedTuple<SizeofLess, double, char, int> t2(t1);
  EXPECT_DOUBLE_EQ(t2.template Get<0>(), 3.14);
  EXPECT_EQ(t2.template Get<1>(), 'x');
  EXPECT_EQ(t2.template Get<2>(), 42);
  // original unchanged
  EXPECT_DOUBLE_EQ(t1.template Get<0>(), 3.14);
}

// --- Move construction ---

TEST(SortedTupleCore, MoveConstruct) {
  SortedTuple<SizeofLess, double, char, int> t1(3.14, 'x', 42);
  SortedTuple<SizeofLess, double, char, int> t2(std::move(t1));
  EXPECT_DOUBLE_EQ(t2.template Get<0>(), 3.14);
  EXPECT_EQ(t2.template Get<1>(), 'x');
  EXPECT_EQ(t2.template Get<2>(), 42);
}

// --- Copy assignment ---

TEST(SortedTupleCore, CopyAssign) {
  SortedTuple<SizeofLess, double, char, int> t1(3.14, 'x', 42);
  SortedTuple<SizeofLess, double, char, int> t2;
  t2 = t1;
  EXPECT_DOUBLE_EQ(t2.template Get<0>(), 3.14);
  EXPECT_EQ(t2.template Get<1>(), 'x');
  EXPECT_EQ(t2.template Get<2>(), 42);
}

// --- Move assignment ---
TEST(SortedTupleCore, MoveAssign) {
  SortedTuple<SizeofLess, double, char, int> t1(3.14, 'x', 42);
  SortedTuple<SizeofLess, double, char, int> t2;
  t2 = std::move(t1);
  EXPECT_DOUBLE_EQ(t2.template Get<0>(), 3.14);
  EXPECT_EQ(t2.template Get<1>(), 'x');
  EXPECT_EQ(t2.template Get<2>(), 42);
}

// --- Mutability via Get ---

TEST(SortedTupleCore, GetByIndexMutate) {
  SortedTuple<SizeofLess, double, char, int> t(3.14, 'x', 42);
  t.template Get<2>() = 99;
  EXPECT_EQ(t.template Get<2>(), 99);
}

TEST(SortedTupleCore, GetByTypeMutate) {
  SortedTuple<SizeofLess, double, char, int> t(3.14, 'x', 42);
  t.template Get<char>() = 'z';
  EXPECT_EQ(t.template Get<char>(), 'z');
}

// --- Swap ---

TEST(SortedTupleCore, Swap) {
  SortedTuple<SizeofLess, double, char, int> t1(1.0, 'a', 10);
  SortedTuple<SizeofLess, double, char, int> t2(2.0, 'b', 20);
  t1.Swap(t2);
  EXPECT_DOUBLE_EQ(t1.template Get<0>(), 2.0);
  EXPECT_EQ(t1.template Get<1>(), 'b');
  EXPECT_EQ(t1.template Get<2>(), 20);
  EXPECT_DOUBLE_EQ(t2.template Get<0>(), 1.0);
  EXPECT_EQ(t2.template Get<1>(), 'a');
  EXPECT_EQ(t2.template Get<2>(), 10);
}

}  // namespace