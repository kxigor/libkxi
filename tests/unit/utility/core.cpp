#include <gtest/gtest.h>

#include <libkxi/utility.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using kxi::utility::CopyConstT;
using kxi::utility::CopyCVT;
using kxi::utility::IndexedType;
using kxi::utility::IsIndexSequenceV;
using kxi::utility::concepts::PerfectCtorGuard;

// =============================================================
// CopyConst — copies only the const qualifier
// =============================================================

TEST(UtilityCopyConst, CopiesConst) {
  EXPECT_TRUE((std::is_same_v<CopyConstT<const int, double>, const double>));
}

TEST(UtilityCopyConst, NoConstStaysClean) {
  EXPECT_TRUE((std::is_same_v<CopyConstT<int, double>, double>));
}

TEST(UtilityCopyConst, VolatileNotCopied) {
  EXPECT_TRUE((std::is_same_v<CopyConstT<volatile int, double>, double>));
}

// =============================================================
// CopyCV — copies const+volatile, strips references on both sides
// =============================================================

TEST(UtilityCopyCV, CopiesConst) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<const int, double>, const double>));
}

TEST(UtilityCopyCV, CopiesVolatile) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<volatile int, double>, volatile double>));
}

TEST(UtilityCopyCV, CopiesConstVolatile) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<const volatile int, double>,
                              const volatile double>));
}

TEST(UtilityCopyCV, StripsReferenceOnSource) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<const int&, double>, const double>));
  EXPECT_TRUE((std::is_same_v<CopyCVT<const int&&, double>, const double>));
}

TEST(UtilityCopyCV, StripsReferenceOnTarget) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<const int, double&>, const double>));
  EXPECT_TRUE((std::is_same_v<CopyCVT<const int, double&&>, const double>));
}

TEST(UtilityCopyCV, NoCVStaysClean) {
  EXPECT_TRUE((std::is_same_v<CopyCVT<int, double>, double>));
}

// =============================================================
// IsIndexSequence
// =============================================================

TEST(UtilityIsIndexSequence, EmptySequence) {
  EXPECT_TRUE((IsIndexSequenceV<std::index_sequence<>>));
}

TEST(UtilityIsIndexSequence, NonEmptySequence) {
  EXPECT_TRUE((IsIndexSequenceV<std::index_sequence<0, 1, 2>>));
  EXPECT_TRUE((IsIndexSequenceV<std::make_index_sequence<5>>));
}

TEST(UtilityIsIndexSequence, RejectsOtherTypes) {
  EXPECT_FALSE(IsIndexSequenceV<int>);
  EXPECT_FALSE((IsIndexSequenceV<std::integer_sequence<int, 0, 1>>));
}

// =============================================================
// PerfectCtorGuard — blocks the variadic ctor from stealing copy/move
// =============================================================

struct Probe {};

TEST(UtilityPerfectCtorGuard, AllowsZeroArgs) {
  EXPECT_TRUE((PerfectCtorGuard<Probe>));
}

TEST(UtilityPerfectCtorGuard, AllowsDifferentSingleArg) {
  EXPECT_TRUE((PerfectCtorGuard<Probe, int>));
  EXPECT_TRUE((PerfectCtorGuard<Probe, std::string>));
}

TEST(UtilityPerfectCtorGuard, BlocksSameType) {
  EXPECT_FALSE((PerfectCtorGuard<Probe, Probe>));
}

TEST(UtilityPerfectCtorGuard, BlocksCVRefOfSameType) {
  EXPECT_FALSE((PerfectCtorGuard<Probe, Probe&>));
  EXPECT_FALSE((PerfectCtorGuard<Probe, const Probe&>));
  EXPECT_FALSE((PerfectCtorGuard<Probe, Probe&&>));
  EXPECT_FALSE((PerfectCtorGuard<Probe, const Probe&&>));
  EXPECT_FALSE((PerfectCtorGuard<Probe, volatile Probe&>));
}

TEST(UtilityPerfectCtorGuard, AllowsMultipleArgsEvenIfSame) {
  // Multi-arg ctor isn't ambiguous with single-arg copy/move ctor.
  EXPECT_TRUE((PerfectCtorGuard<Probe, Probe, int>));
  EXPECT_TRUE((PerfectCtorGuard<Probe, Probe, Probe>));
}

// =============================================================
// IndexedType — used as the base brick for Tuple
// =============================================================

TEST(UtilityIndexedType, ExposesHolderNumberAndValueT) {
  using T = IndexedType<3, double>;
  EXPECT_EQ(T::kHolderNumber, 3u);
  EXPECT_TRUE((std::is_same_v<T::ValueT, double>));
}

TEST(UtilityIndexedType, DefaultConstructsValue) {
  IndexedType<0, int> v;
  EXPECT_EQ(v.Value(), 0);
}

TEST(UtilityIndexedType, ValueConstructs) {
  IndexedType<0, int> v(42);
  EXPECT_EQ(v.Value(), 42);
}

TEST(UtilityIndexedType, ValueIsMutableForNonConst) {
  IndexedType<0, int> v(10);
  v.Value() = 20;
  EXPECT_EQ(v.Value(), 20);
  EXPECT_TRUE((std::is_same_v<decltype(v.Value()), int&>));
}

TEST(UtilityIndexedType, ValueIsConstForConst) {
  const IndexedType<0, int> v(10);
  EXPECT_TRUE((std::is_same_v<decltype(v.Value()), const int&>));
}

TEST(UtilityIndexedType, ValueIsVolatileForVolatile) {
  volatile IndexedType<0, int> v(10);
  EXPECT_TRUE((std::is_same_v<decltype(v.Value()), volatile int&>));
}

TEST(UtilityIndexedType, ValueIsConstVolatileForConstVolatile) {
  const volatile IndexedType<0, int> v(10);
  EXPECT_TRUE((std::is_same_v<decltype(v.Value()), const volatile int&>));
}

TEST(UtilityIndexedType, DistinctIndicesYieldDistinctTypes) {
  EXPECT_FALSE((std::is_same_v<IndexedType<0, int>, IndexedType<1, int>>));
}

TEST(UtilityIndexedType, IndexedTypeCopyMove) {
  IndexedType<0, std::string> a("hello");
  IndexedType<0, std::string> b(a);  // copy
  EXPECT_EQ(b.Value(), "hello");
  EXPECT_EQ(a.Value(), "hello");

  IndexedType<0, std::string> c(std::move(a));  // move
  EXPECT_EQ(c.Value(), "hello");
}

// PerfectCtorGuard plumbing: variadic ctor of IndexedType must NOT shadow
// the copy ctor — copying an lvalue must call the copy ctor, not construct
// the value from the IndexedType itself.
TEST(UtilityIndexedType, CopyCtorWinsOverPerfectForwardingCtor) {
  // If the variadic ctor stole the copy, this would try to construct
  // std::string from another IndexedType<0, std::string>, which has no
  // conversion path and would fail to compile.
  IndexedType<0, std::string> a("abc");
  IndexedType<0, std::string> b(a);
  EXPECT_EQ(a.Value(), "abc");
  EXPECT_EQ(b.Value(), "abc");
}

}  // namespace
