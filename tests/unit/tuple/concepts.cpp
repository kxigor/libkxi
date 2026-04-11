#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <utility>

namespace {

using kxi::tuple::IsTupleV;
using kxi::tuple::FlatTuple;

TEST(TupleConcepts, TupleSpecIsDetected) {
  EXPECT_TRUE(IsTupleV<FlatTuple<>>);
  EXPECT_TRUE(IsTupleV<FlatTuple<int>>);
  EXPECT_TRUE((IsTupleV<FlatTuple<int, double, char>>));
}

TEST(TupleConcepts, NonTupleSpecIsRejected) {
  EXPECT_FALSE(IsTupleV<int>);
  EXPECT_FALSE(IsTupleV<double>);
  EXPECT_FALSE(IsTupleV<void>);
  EXPECT_FALSE((IsTupleV<std::pair<int, double>>));
}

}  // namespace