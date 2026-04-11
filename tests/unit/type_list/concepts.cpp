#include <gtest/gtest.h>

#include <libkxi/heterogeneous.hpp>
#include <tuple>
#include <utility>

namespace {

using kxi::het::IsHeterogeneousV;

template <typename...>
struct TList {};

TEST(HeterogeneousConcepts, SpecIsDetected) {
  EXPECT_TRUE((IsHeterogeneousV<TList, TList<>>));
  EXPECT_TRUE((IsHeterogeneousV<TList, TList<int>>));
  EXPECT_TRUE((IsHeterogeneousV<TList, TList<int, double, char>>));

  EXPECT_TRUE((IsHeterogeneousV<std::tuple, std::tuple<int, float>>));
}

TEST(HeterogeneousConcepts, NonSpecIsRejected) {
  EXPECT_FALSE((IsHeterogeneousV<TList, int>));
  EXPECT_FALSE((IsHeterogeneousV<TList, double>));
  EXPECT_FALSE((IsHeterogeneousV<TList, void>));
  EXPECT_FALSE((IsHeterogeneousV<TList, std::pair<int, double>>));

  EXPECT_FALSE((IsHeterogeneousV<TList, std::tuple<int, double>>));
}

}  // namespace