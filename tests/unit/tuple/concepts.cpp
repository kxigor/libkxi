#include <gtest/gtest.h>

#include <libkxi/tuple/flat.hpp>
#include <utility>

namespace {

using kxi::tuple::flat::IsFlatTupleV;
using kxi::tuple::flat::FlatTuple;

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