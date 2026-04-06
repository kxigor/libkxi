#include <gtest/gtest.h>

#include <cstddef>
#include <libkxi/sorted_tuple.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::sorted_tuple::SortedTuple;

template <typename LHS, typename RHS>
struct SizeofLess {
  static constexpr const bool value = sizeof(LHS) < sizeof(RHS);
};

// ======================== Qualifier tests ========================

// --- Get by index: value categories ---

TEST(SortedTupleQualifiers, GetByIndex_Lvalue) {
  SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), double&>));
  v = 99.0;
  EXPECT_DOUBLE_EQ(t.template Get<0>(), 99.0);
}

TEST(SortedTupleQualifiers, GetByIndex_ConstLvalue) {
  const SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const double&>));
}

TEST(SortedTupleQualifiers, GetByIndex_Rvalue) {
  SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = std::move(t).template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), double&&>));
}

TEST(SortedTupleQualifiers, GetByIndex_ConstRvalue) {
  const SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = std::move(t).template Get<0>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const double&&>));
}

// --- Get by type: value categories ---

TEST(SortedTupleQualifiers, GetByType_Lvalue) {
  SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<char>();
  EXPECT_TRUE((std::is_same_v<decltype(v), char&>));
}

TEST(SortedTupleQualifiers, GetByType_ConstLvalue) {
  const SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<char>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const char&>));
}

TEST(SortedTupleQualifiers, GetByType_Rvalue) {
  SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = std::move(t).template Get<char>();
  EXPECT_TRUE((std::is_same_v<decltype(v), char&&>));
}

TEST(SortedTupleQualifiers, GetByType_ConstRvalue) {
  const SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = std::move(t).template Get<char>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const char&&>));
}

// --- Volatile ---

TEST(SortedTupleQualifiers, GetByIndex_VolatileLvalue) {
  volatile SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<1>();
  EXPECT_TRUE((std::is_same_v<decltype(v), volatile char&>));
}

TEST(SortedTupleQualifiers, GetByIndex_ConstVolatileLvalue) {
  const volatile SortedTuple<SizeofLess, double, char, int> t(1.0, 'a', 10);
  decltype(auto) v = t.template Get<1>();
  EXPECT_TRUE((std::is_same_v<decltype(v), const volatile char&>));
}

// ======================== Move counting ========================

struct Counters {
  std::size_t default_ctor = 0;
  std::size_t copy_ctor = 0;
  std::size_t move_ctor = 0;
  std::size_t copy_assign = 0;
  std::size_t move_assign = 0;

  void Reset() { *this = Counters{}; }
};

static Counters g_counters;  // NOLINT

struct TrackedSmall {
  char data = 0;

  TrackedSmall() { ++g_counters.default_ctor; }
  explicit TrackedSmall(char v) : data(v) { ++g_counters.default_ctor; }
  TrackedSmall(const TrackedSmall& o) : data(o.data) { ++g_counters.copy_ctor; }
  TrackedSmall(TrackedSmall&& o) noexcept : data(o.data) {
    o.data = 0;
    ++g_counters.move_ctor;
  }
  TrackedSmall& operator=(const TrackedSmall& o) {
    data = o.data;
    ++g_counters.copy_assign;
    return *this;
  }
  TrackedSmall& operator=(TrackedSmall&& o) noexcept {
    data = o.data;
    o.data = 0;
    ++g_counters.move_assign;
    return *this;
  }
};

struct TrackedLarge {
  double data = 0;
  TrackedLarge() { ++g_counters.default_ctor; }
  explicit TrackedLarge(double v) : data(v) { ++g_counters.default_ctor; }
  TrackedLarge(const TrackedLarge& o) : data(o.data) { ++g_counters.copy_ctor; }
  TrackedLarge(TrackedLarge&& o) noexcept : data(o.data) {
    o.data = 0;
    ++g_counters.move_ctor;
  }
  TrackedLarge& operator=(const TrackedLarge& o) {
    data = o.data;
    ++g_counters.copy_assign;
    return *this;
  }
  TrackedLarge& operator=(TrackedLarge&& o) noexcept {
    data = o.data;
    o.data = 0;
    ++g_counters.move_assign;
    return *this;
  }
};

// sizeof(TrackedSmall) = 1, sizeof(TrackedLarge) = 8
// Original order: Large, Small => internally sorted: Small, Large

class SortedTupleMoveTest : public ::testing::Test {
 protected:
  void SetUp() override { g_counters.Reset(); }
};

TEST_F(SortedTupleMoveTest, ConstructFromRvalues) {
  TrackedLarge large(1.0);
  TrackedSmall small('a');
  g_counters.Reset();

  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t(std::move(large),
                                                        std::move(small));

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_DOUBLE_EQ(t.template Get<0>().data, 1.0);
  EXPECT_EQ(t.template Get<1>().data, 'a');
}

TEST_F(SortedTupleMoveTest, ConstructFromLvaluesCopies) {
  TrackedLarge large(1.0);
  TrackedSmall small('a');
  g_counters.Reset();

  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t(large, small);

  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_GE(g_counters.copy_ctor, 2u);
  EXPECT_DOUBLE_EQ(t.template Get<0>().data, 1.0);
  EXPECT_EQ(t.template Get<1>().data, 'a');
}

TEST_F(SortedTupleMoveTest, CopyConstructCopies) {
  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t1(TrackedLarge(1.0),
                                                         TrackedSmall('a'));
  g_counters.Reset();

  auto t2 = t1;

  EXPECT_EQ(g_counters.copy_ctor, 2u);
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_DOUBLE_EQ(t2.template Get<0>().data, 1.0);
  EXPECT_EQ(t2.template Get<1>().data, 'a');
}

TEST_F(SortedTupleMoveTest, MoveConstructMoves) {
  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t1(TrackedLarge(1.0),
                                                         TrackedSmall('a'));
  g_counters.Reset();

  auto t2 = std::move(t1);

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 2u);
  EXPECT_DOUBLE_EQ(t2.template Get<0>().data, 1.0);
  EXPECT_EQ(t2.template Get<1>().data, 'a');
}

TEST_F(SortedTupleMoveTest, GetLvalueNoCopyMove) {
  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t(TrackedLarge(1.0),
                                                        TrackedSmall('a'));
  g_counters.Reset();

  decltype(auto) ref = t.template Get<0>();

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_assign, 0u);
  EXPECT_DOUBLE_EQ(ref.data, 1.0);
}

TEST_F(SortedTupleMoveTest, SwapCounting) {
  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t1(TrackedLarge(1.0),
                                                         TrackedSmall('a'));
  SortedTuple<SizeofLess, TrackedLarge, TrackedSmall> t2(TrackedLarge(2.0),
                                                         TrackedSmall('b'));
  g_counters.Reset();

  t1.Swap(t2);

  // 2 elements × std::swap (1 move_ctor + 2 move_assign each)
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_ctor, 2u);
  EXPECT_EQ(g_counters.move_assign, 4u);

  EXPECT_DOUBLE_EQ(t1.template Get<0>().data, 2.0);
  EXPECT_EQ(t1.template Get<1>().data, 'b');
  EXPECT_DOUBLE_EQ(t2.template Get<0>().data, 1.0);
  EXPECT_EQ(t2.template Get<1>().data, 'a');
}

}  // namespace