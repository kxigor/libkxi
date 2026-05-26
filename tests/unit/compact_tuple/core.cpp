#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/tuple/compact.hpp>
#include <libkxi/tuple/flat.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::tuple::compact::CompactTuple;

// --- Default construction ---

TEST(CompactTupleCore, DefaultConstruct) {
  CompactTuple<int, double> t;
  EXPECT_EQ(t.template Get<0>(), 0);
  EXPECT_EQ(t.template Get<1>(), 0.0);
}

// --- Value construction: external order is preserved by Get<I> regardless
// of internal reordering by the compact predicate ---

TEST(CompactTupleCore, ValueConstructPreservesOrigninalGetOrder) {
  CompactTuple<std::int8_t, std::int32_t, std::int16_t> t(std::int8_t{1}, 2,
                                                          std::int16_t{3});
  EXPECT_EQ(t.template Get<0>(), 1);
  EXPECT_EQ(t.template Get<1>(), 2);
  EXPECT_EQ(t.template Get<2>(), 3);
}

// --- Get by type ---

TEST(CompactTupleCore, GetByType) {
  CompactTuple<std::int8_t, std::int32_t, std::int16_t> t(std::int8_t{1}, 2,
                                                          std::int16_t{3});
  EXPECT_EQ((t.template Get<std::int8_t>()), 1);
  EXPECT_EQ((t.template Get<std::int32_t>()), 2);
  EXPECT_EQ((t.template Get<std::int16_t>()), 3);
}

// --- Copy / Move ---

TEST(CompactTupleCore, CopyConstruct) {
  CompactTuple<int, std::string> a(10, "abc");
  CompactTuple<int, std::string> b(a);
  EXPECT_EQ(b.template Get<0>(), 10);
  EXPECT_EQ(b.template Get<1>(), "abc");
}

TEST(CompactTupleCore, MoveConstruct) {
  CompactTuple<int, std::string> a(10, "abc");
  CompactTuple<int, std::string> b(std::move(a));
  EXPECT_EQ(b.template Get<0>(), 10);
  EXPECT_EQ(b.template Get<1>(), "abc");
}

// --- Swap ---

TEST(CompactTupleCore, Swap) {
  CompactTuple<int, std::string> a(1, "a");
  CompactTuple<int, std::string> b(2, "b");
  a.Swap(b);
  EXPECT_EQ(a.template Get<0>(), 2);
  EXPECT_EQ(a.template Get<1>(), "b");
  EXPECT_EQ(b.template Get<0>(), 1);
  EXPECT_EQ(b.template Get<1>(), "a");
}

// --- Layout: the whole point of CompactTuple is to remove padding by
// reordering. Compare against flat::Tuple with the same arguments. ---

TEST(CompactTupleCore, ReducesPaddingVsFlat) {
  using Flat = kxi::tuple::flat::Tuple<std::int8_t, std::int32_t, std::int8_t>;
  using Compact = CompactTuple<std::int8_t, std::int32_t, std::int8_t>;
  EXPECT_LT(sizeof(Compact), sizeof(Flat));
}

TEST(CompactTupleCore, EqualToFlatForAlreadyOptimalOrder) {
  // alignment-descending order is already optimal for the flat layout.
  using Flat = kxi::tuple::flat::Tuple<std::int32_t, std::int16_t, std::int8_t>;
  using Compact = CompactTuple<std::int32_t, std::int16_t, std::int8_t>;
  EXPECT_EQ(sizeof(Compact), sizeof(Flat));
}

}  // namespace
