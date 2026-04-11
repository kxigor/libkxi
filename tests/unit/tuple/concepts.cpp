#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <utility>

namespace {

using kxi::tuple::IsFlatTupleV;
using kxi::tuple::FlatTuple;

TEST(TupleConcepts, TupleSpecIsDetected) {
  EXPECT_TRUE(IsFlatTupleV<FlatTuple<>>);
  EXPECT_TRUE(IsFlatTupleV<FlatTuple<int>>);
  EXPECT_TRUE((IsFlatTupleV<FlatTuple<int, double, char>>));
}

TEST(TupleConcepts, NonTupleSpecIsRejected) {
  EXPECT_FALSE(IsFlatTupleV<int>);
  EXPECT_FALSE(IsFlatTupleV<double>);
  EXPECT_FALSE(IsFlatTupleV<void>);
  EXPECT_FALSE((IsFlatTupleV<std::pair<int, double>>));
}

}  // namespace