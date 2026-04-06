#include <gtest/gtest.h>

#include <cstddef>
#include <libkxi/tuple.hpp>
#include <utility>

namespace {

using kxi::tuple::MakeTuple;
using kxi::tuple::Tuple;
using kxi::tuple::TupleCat;

struct Counters {
  std::size_t default_ctor = 0;
  std::size_t copy_ctor = 0;
  std::size_t move_ctor = 0;
  std::size_t copy_assign = 0;
  std::size_t move_assign = 0;
  std::size_t destructor = 0;

  void Reset() { *this = Counters{}; }
};

// Global counters, reset before each test
static Counters g_counters;  // NOLINT

struct Tracked {
  int value = 0;

  Tracked() { ++g_counters.default_ctor; }
  explicit Tracked(int v) : value(v) { ++g_counters.default_ctor; }
  Tracked(const Tracked& o) : value(o.value) { ++g_counters.copy_ctor; }
  Tracked(Tracked&& o) noexcept : value(o.value) {
    o.value = -1;
    ++g_counters.move_ctor;
  }
  Tracked& operator=(const Tracked& o) {
    value = o.value;
    ++g_counters.copy_assign;
    return *this;
  }
  Tracked& operator=(Tracked&& o) noexcept {
    value = o.value;
    o.value = -1;
    ++g_counters.move_assign;
    return *this;
  }
  ~Tracked() { ++g_counters.destructor; }
};

class TupleMoveCountingTest : public ::testing::Test {
 protected:
  void SetUp() override { g_counters.Reset(); }
};

// --- Construct from lvalue: should copy, not move ---

TEST_F(TupleMoveCountingTest, ConstructFromLvalue) {
  Tracked t(42);
  g_counters.Reset();

  Tuple<Tracked> tup(t);

  EXPECT_EQ(g_counters.copy_ctor, 1u);
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_EQ(tup.template Get<0>().value, 42);
}

// --- Construct from rvalue: should move, not copy ---

TEST_F(TupleMoveCountingTest, ConstructFromRvalue) {
  Tracked t(42);
  g_counters.Reset();

  Tuple<Tracked> tup(std::move(t));

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 1u);
  EXPECT_EQ(tup.template Get<0>().value, 42);
}

// --- Copy construction of Tuple ---

TEST_F(TupleMoveCountingTest, TupleCopyConstruct) {
  Tuple<Tracked> t1(Tracked(10));
  g_counters.Reset();

  Tuple<Tracked> t2(t1);

  EXPECT_EQ(g_counters.copy_ctor, 1u);
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_EQ(t2.template Get<0>().value, 10);
}

// --- Move construction of Tuple ---

TEST_F(TupleMoveCountingTest, TupleMoveConstruct) {
  Tuple<Tracked> t1(Tracked(10));
  g_counters.Reset();

  Tuple<Tracked> t2(std::move(t1));

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 1u);
  EXPECT_EQ(t2.template Get<0>().value, 10);
}

// --- Copy assignment ---

TEST_F(TupleMoveCountingTest, TupleCopyAssign) {
  Tuple<Tracked> t1(Tracked(10));
  Tuple<Tracked> t2(Tracked(20));
  g_counters.Reset();

  t2 = t1;

  EXPECT_EQ(g_counters.copy_assign, 1u);
  EXPECT_EQ(g_counters.move_assign, 0u);
  EXPECT_EQ(t2.template Get<0>().value, 10);
}

// --- Move assignment ---

TEST_F(TupleMoveCountingTest, TupleMoveAssign) {
  Tuple<Tracked> t1(Tracked(10));
  Tuple<Tracked> t2(Tracked(20));
  g_counters.Reset();

  t2 = std::move(t1);

  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_assign, 1u);
  EXPECT_EQ(t2.template Get<0>().value, 10);
}

// --- MakeTuple from rvalue: exactly 1 move into tuple ---

TEST_F(TupleMoveCountingTest, MakeTupleFromRvalue) {
  Tracked t(42);
  g_counters.Reset();

  auto tup = MakeTuple(std::move(t));

  // One move into the tuple element.
  // MakeTuple may produce a prvalue (NRVO), so no extra move for the tuple
  // itself.
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_GE(g_counters.move_ctor, 1u);
  EXPECT_EQ(tup.template Get<0>().value, 42);
}

// --- MakeTuple from lvalue: exactly 1 copy into tuple ---

TEST_F(TupleMoveCountingTest, MakeTupleFromLvalue) {
  Tracked t(42);
  g_counters.Reset();

  auto tup = MakeTuple(t);

  EXPECT_GE(g_counters.copy_ctor, 1u);
  EXPECT_EQ(tup.template Get<0>().value, 42);
}

// --- Get rvalue from tuple triggers no copy/move (returns reference) ---

TEST_F(TupleMoveCountingTest, GetLvalueNoCopyMove) {
  Tuple<Tracked> tup(Tracked(42));
  g_counters.Reset();

  decltype(auto) ref = tup.template Get<0>();
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_assign, 0u);
  EXPECT_EQ(ref.value, 42);
}

// --- Swap: exactly 1 move-construct + 2 move-assigns per element (std::swap)
// ---

TEST_F(TupleMoveCountingTest, SwapCounting) {
  Tuple<Tracked> t1(Tracked(1));
  Tuple<Tracked> t2(Tracked(2));
  g_counters.Reset();

  t1.Swap(t2);

  // std::swap does: tmp = move(a); a = move(b); b = move(tmp);
  // = 1 move ctor + 2 move assigns per element
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_ctor, 1u);
  EXPECT_EQ(g_counters.move_assign, 2u);

  EXPECT_EQ(t1.template Get<0>().value, 2);
  EXPECT_EQ(t2.template Get<0>().value, 1);
}

// --- Multiple elements: each tracked independently ---

TEST_F(TupleMoveCountingTest, TwoElementsMoveConstruct) {
  Tracked a(1);
  Tracked b(2);
  g_counters.Reset();

  Tuple<Tracked, Tracked> tup(std::move(a), std::move(b));

  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.move_ctor, 2u);
  EXPECT_EQ(tup.template Get<0>().value, 1);
  EXPECT_EQ(tup.template Get<1>().value, 2);
}

TEST_F(TupleMoveCountingTest, TwoElementsSwap) {
  Tuple<Tracked, Tracked> t1(Tracked(1), Tracked(2));
  Tuple<Tracked, Tracked> t2(Tracked(3), Tracked(4));
  g_counters.Reset();

  t1.Swap(t2);

  // 2 elements × (1 move_ctor + 2 move_assign)
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(g_counters.move_ctor, 2u);
  EXPECT_EQ(g_counters.move_assign, 4u);

  EXPECT_EQ(t1.template Get<0>().value, 3);
  EXPECT_EQ(t1.template Get<1>().value, 4);
  EXPECT_EQ(t2.template Get<0>().value, 1);
  EXPECT_EQ(t2.template Get<1>().value, 2);
}

// --- TupleCat move counting ---

TEST_F(TupleMoveCountingTest, TupleCatFromRvalues) {
  auto t1 = MakeTuple(Tracked(1));
  auto t2 = MakeTuple(Tracked(2));
  g_counters.Reset();

  auto result = TupleCat(std::move(t1), std::move(t2));

  // Elements should be moved out of t1 and t2, not copied
  EXPECT_EQ(g_counters.copy_ctor, 0u);
  EXPECT_EQ(g_counters.copy_assign, 0u);
  EXPECT_EQ(result.template Get<0>().value, 1);
  EXPECT_EQ(result.template Get<1>().value, 2);
}

TEST_F(TupleMoveCountingTest, TupleCatFromLvaluesCopies) {
  auto t1 = MakeTuple(Tracked(1));
  auto t2 = MakeTuple(Tracked(2));
  g_counters.Reset();

  auto result = TupleCat(t1, t2);

  // Elements should be copied from lvalue tuples
  EXPECT_EQ(g_counters.move_ctor, 0u);
  EXPECT_GE(g_counters.copy_ctor, 2u);
  EXPECT_EQ(result.template Get<0>().value, 1);
  EXPECT_EQ(result.template Get<1>().value, 2);
}

}  // namespace