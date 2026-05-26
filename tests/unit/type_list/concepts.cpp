#include <gtest/gtest.h>

#include <array>
#include <libkxi/meta.hpp>
#include <libkxi/tuple/flat.hpp>
#include <tuple>
#include <utility>
#include <vector>

namespace {

using kxi::meta::IsAllSameShellsV;
using kxi::meta::IsDistinctV;
using kxi::meta::IsTListV;

template <typename...>
struct TList {};

template <typename...>
struct OtherList {};

// =============================================================
// IsTList / concepts::TList
// =============================================================

TEST(MetaIsTList, AcceptsEmptyShell) { EXPECT_TRUE((IsTListV<TList<>>)); }

TEST(MetaIsTList, AcceptsShellWithTypes) {
  EXPECT_TRUE((IsTListV<TList<int>>));
  EXPECT_TRUE((IsTListV<TList<int, double, char>>));
}

TEST(MetaIsTList, AcceptsStdTuple) {
  EXPECT_TRUE((IsTListV<std::tuple<>>));
  EXPECT_TRUE((IsTListV<std::tuple<int, double>>));
}

TEST(MetaIsTList, AcceptsFlatTuple) {
  EXPECT_TRUE((IsTListV<kxi::tuple::flat::Tuple<>>));
  EXPECT_TRUE((IsTListV<kxi::tuple::flat::Tuple<int, double>>));
}

TEST(MetaIsTList, StripsCVRef) {
  EXPECT_TRUE((IsTListV<const TList<int>>));
  EXPECT_TRUE((IsTListV<volatile TList<int>>));
  EXPECT_TRUE((IsTListV<TList<int>&>));
  EXPECT_TRUE((IsTListV<TList<int>&&>));
  EXPECT_TRUE((IsTListV<const volatile TList<int>&>));
}

TEST(MetaIsTList, RejectsNonTemplateTypes) {
  EXPECT_FALSE(IsTListV<int>);
  EXPECT_FALSE(IsTListV<void>);
  EXPECT_FALSE((IsTListV<int[3]>));
}

TEST(MetaIsTList, RejectsFixedArityShell) {
  // std::pair has a fixed-arity head template<class T1, class T2>
  EXPECT_FALSE((IsTListV<std::pair<int, double>>));
}

TEST(MetaIsTList, RejectsNonTypeTemplateParams) {
  // std::array<T, N> mixes a type and a non-type parameter
  EXPECT_FALSE((IsTListV<std::array<int, 3>>));
  EXPECT_FALSE((IsTListV<std::index_sequence<0, 1>>));
}

// std::vector<T, Alloc> matches Shell<Args...> via default args, but the
// re-instantiation Shell<PackTester<>, PackTester<int>, PackTester<Alloc>>
// fails (vector wants Alloc to satisfy allocator concept). Document that.
TEST(MetaIsTList, RejectsVectorAsTList) {
  EXPECT_FALSE((IsTListV<std::vector<int>>));
}

// =============================================================
// IsDistinct / concepts::Distinct
// =============================================================

TEST(MetaIsDistinct, EmptyListIsDistinct) {
  EXPECT_TRUE((IsDistinctV<TList<>>));
}

TEST(MetaIsDistinct, SingleElementIsDistinct) {
  EXPECT_TRUE((IsDistinctV<TList<int>>));
}

TEST(MetaIsDistinct, AllUniqueIsDistinct) {
  EXPECT_TRUE((IsDistinctV<TList<int, double, char>>));
}

TEST(MetaIsDistinct, DuplicatesNotDistinct) {
  EXPECT_FALSE((IsDistinctV<TList<int, int>>));
  EXPECT_FALSE((IsDistinctV<TList<int, double, int>>));
  EXPECT_FALSE((IsDistinctV<TList<int, double, double>>));
}

TEST(MetaIsDistinct, StripsCVRef) {
  EXPECT_TRUE((IsDistinctV<const TList<int, double>>));
  EXPECT_TRUE((IsDistinctV<TList<int, double>&>));
  EXPECT_FALSE((IsDistinctV<const TList<int, int>&>));
}

TEST(MetaIsDistinct, TreatsCVQualifiedTypesAsDistinct) {
  // const T and T are different types
  EXPECT_TRUE((IsDistinctV<TList<int, const int>>));
  EXPECT_TRUE((IsDistinctV<TList<int, int&>>));
  EXPECT_TRUE((IsDistinctV<TList<int, int&&>>));
}

// =============================================================
// IsAllSameShells / concepts::AllSameShells
// =============================================================

TEST(MetaIsAllSameShells, SingleListIsTrivial) {
  EXPECT_TRUE((IsAllSameShellsV<TList<int>>));
}

TEST(MetaIsAllSameShells, SameShellMultipleLists) {
  EXPECT_TRUE((IsAllSameShellsV<TList<int>, TList<double>>));
  EXPECT_TRUE(
      (IsAllSameShellsV<TList<int>, TList<double>, TList<char, float>>));
}

TEST(MetaIsAllSameShells, DifferentShellsRejected) {
  EXPECT_FALSE((IsAllSameShellsV<TList<int>, OtherList<int>>));
  EXPECT_FALSE((IsAllSameShellsV<TList<int>, std::tuple<int>>));
}

TEST(MetaIsAllSameShells, EmptyListsStillSame) {
  EXPECT_TRUE((IsAllSameShellsV<TList<>, TList<>>));
  EXPECT_FALSE((IsAllSameShellsV<TList<>, OtherList<>>));
}

TEST(MetaIsAllSameShells, StripsCVRef) {
  EXPECT_TRUE((IsAllSameShellsV<const TList<int>&, TList<double>&&>));
}

}  // namespace
