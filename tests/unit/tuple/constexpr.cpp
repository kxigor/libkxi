#include <gtest/gtest.h>

#include <libkxi/tuple/flat.hpp>

namespace {

using kxi::tuple::flat::Tuple;

TEST(TupleConstexpr, DefaultConstructAndGet) {
  constexpr Tuple<int, double> t{};
  static_assert(t.template Get<0>() == 0);
  static_assert(t.template Get<1>() == 0.0);
}

TEST(TupleConstexpr, ValueConstructAndGet) {
  constexpr Tuple<int, double> t(1, 2.0);
  static_assert(t.template Get<0>() == 1);
  static_assert(t.template Get<1>() == 2.0);
}

TEST(TupleConstexpr, GetByType) {
  constexpr Tuple<int, double> t(7, 3.5);
  static_assert(t.template Get<int>() == 7);
  static_assert(t.template Get<double>() == 3.5);
}

TEST(TupleConstexpr, SumOfElements) {
  constexpr Tuple<int, int, int> t(1, 2, 3);
  static_assert(
      t.template Get<0>() + t.template Get<1>() + t.template Get<2>() == 6);
}

}  // namespace
