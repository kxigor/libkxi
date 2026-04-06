#include <gtest/gtest.h>

#include <libkxi/sorted_tuple.hpp>
#include <utility>

namespace {

using kxi::sorted_tuple::IsSortedTupleSpecV;
using kxi::sorted_tuple::SortedTuple;

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

TEST(SortedTupleConcepts, SortedTupleSpecIsDetected) {
  EXPECT_TRUE((IsSortedTupleSpecV<SortedTuple<SizeofLess>>));
  EXPECT_TRUE((IsSortedTupleSpecV<SortedTuple<SizeofLess, int>>));
  EXPECT_TRUE((IsSortedTupleSpecV<SortedTuple<SizeofLess, int, double, char>>));
}

TEST(SortedTupleConcepts, SortedTupleSpecWithCVRef) {
  EXPECT_TRUE((IsSortedTupleSpecV<const SortedTuple<SizeofLess, int>>));
  EXPECT_TRUE((IsSortedTupleSpecV<SortedTuple<SizeofLess, int>&>));
  EXPECT_TRUE((IsSortedTupleSpecV<const SortedTuple<SizeofLess, int>&>));
  EXPECT_TRUE((IsSortedTupleSpecV<SortedTuple<SizeofLess, int>&&>));
  EXPECT_TRUE((IsSortedTupleSpecV<const SortedTuple<SizeofLess, int>&&>));
  EXPECT_TRUE((IsSortedTupleSpecV<volatile SortedTuple<SizeofLess, int>>));
  EXPECT_TRUE(
      (IsSortedTupleSpecV<const volatile SortedTuple<SizeofLess, int>&>));
}

TEST(SortedTupleConcepts, NonSortedTupleSpecIsRejected) {
  EXPECT_FALSE(IsSortedTupleSpecV<int>);
  EXPECT_FALSE(IsSortedTupleSpecV<double>);
  EXPECT_FALSE((IsSortedTupleSpecV<std::pair<int, double>>));
  EXPECT_FALSE((IsSortedTupleSpecV<kxi::tuple::Tuple<int, double>>));
}

}  // namespace