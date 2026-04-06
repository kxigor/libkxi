#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <utility>

namespace {

using kxi::tuple::IsTupleSpecV;
using kxi::tuple::Tuple;

TEST(TupleConcepts, TupleSpecIsDetected) {
  EXPECT_TRUE(IsTupleSpecV<Tuple<>>);
  EXPECT_TRUE(IsTupleSpecV<Tuple<int>>);
  EXPECT_TRUE((IsTupleSpecV<Tuple<int, double, char>>));
}

TEST(TupleConcepts, NonTupleSpecIsRejected) {
  EXPECT_FALSE(IsTupleSpecV<int>);
  EXPECT_FALSE(IsTupleSpecV<double>);
  EXPECT_FALSE(IsTupleSpecV<void>);
  EXPECT_FALSE((IsTupleSpecV<std::pair<int, double>>));
}

}  // namespace