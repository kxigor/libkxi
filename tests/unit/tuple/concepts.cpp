#include <gtest/gtest.h>

#include <libkxi/tuple/flat.hpp>
#include <libkxi/tuple/like.hpp>
#include <tuple>
#include <utility>

namespace {

using kxi::tuple::flat::Tuple;
using kxi::tuple::like::concepts::DistinctTupleLike;
using kxi::tuple::like::concepts::SwappableTuple;
using kxi::tuple::like::concepts::TupleLike;

// =============================================================
// TupleLike — must expose meta::SizeV and .Get<I>() for every I
// =============================================================

TEST(LikeTupleLikeConcept, AcceptsFlatTupleEmpty) {
  EXPECT_TRUE(TupleLike<Tuple<>>);
}

TEST(LikeTupleLikeConcept, AcceptsFlatTuple) {
  EXPECT_TRUE((TupleLike<Tuple<int, double>>));
  EXPECT_TRUE((TupleLike<Tuple<int, double, char>>));
}

TEST(LikeTupleLikeConcept, RejectsRawTypes) {
  EXPECT_FALSE(TupleLike<int>);
  EXPECT_FALSE(TupleLike<void>);
}

TEST(LikeTupleLikeConcept, RejectsStdTuple) {
  // std::tuple uses the free std::get, not .Get<I>().
  EXPECT_FALSE((TupleLike<std::tuple<int, double>>));
}

TEST(LikeTupleLikeConcept, RejectsStdPair) {
  EXPECT_FALSE((TupleLike<std::pair<int, double>>));
}

// =============================================================
// SwappableTuple — TupleLike + has .Swap(other)
// =============================================================

TEST(LikeSwappableTupleConcept, AcceptsFlatTuple) {
  EXPECT_TRUE((SwappableTuple<Tuple<int, double>>));
}

TEST(LikeSwappableTupleConcept, RejectsRawType) {
  EXPECT_FALSE(SwappableTuple<int>);
}

// =============================================================
// DistinctTupleLike — TupleLike + .Get<T>() for every type T
// =============================================================

TEST(LikeDistinctTupleLikeConcept, AcceptsTupleWithUniqueTypes) {
  EXPECT_TRUE((DistinctTupleLike<Tuple<int, double, char>>));
}

TEST(LikeDistinctTupleLikeConcept, RejectsRawType) {
  EXPECT_FALSE(DistinctTupleLike<int>);
}

TEST(LikeDistinctTupleLikeConcept, RejectsStdTuple) {
  EXPECT_FALSE((DistinctTupleLike<std::tuple<int, double>>));
}

TEST(LikeDistinctTupleLikeConcept, RejectsTupleWithDuplicateTypes) {
  EXPECT_FALSE((DistinctTupleLike<Tuple<int, int>>));
}

}  // namespace
