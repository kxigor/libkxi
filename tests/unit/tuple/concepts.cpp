#include <gtest/gtest.h>

#include <libkxi/tuple/flat.hpp>
#include <utility>

namespace {

using kxi::tuple::flat::IsTupleV;
using kxi::tuple::flat::Tuple;

TEST(TupleConcepts, TupleSpecIsDetected) {
  EXPECT_TRUE(IsTupleV<Tuple<>>);
  EXPECT_TRUE(IsTupleV<Tuple<int>>);
  EXPECT_TRUE((IsTupleV<Tuple<int, double, char>>));
}

TEST(TupleConcepts, NonTupleSpecIsRejected) {
  EXPECT_FALSE(IsTupleV<int>);
  EXPECT_FALSE(IsTupleV<double>);
  EXPECT_FALSE(IsTupleV<void>);
  EXPECT_FALSE((IsTupleV<std::pair<int, double>>));
}

}  // namespace