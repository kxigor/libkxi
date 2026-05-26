#include <gtest/gtest.h>

#include <libkxi/meta.hpp>
#include <type_traits>

namespace {

using kxi::meta::pack::CountV;
using kxi::meta::pack::IndexOfV;
using kxi::meta::pack::IsDistinctV;
using kxi::meta::pack::IsPackHolderV;
using kxi::meta::pack::PackHolder;
using kxi::meta::pack::TypeAtT;

// =============================================================
// IsPackHolder
// =============================================================

TEST(MetaPackIsPackHolder, AcceptsEmpty) {
  EXPECT_TRUE((IsPackHolderV<PackHolder<>>));
}

TEST(MetaPackIsPackHolder, AcceptsWithTypes) {
  EXPECT_TRUE((IsPackHolderV<PackHolder<int, double, char>>));
}

TEST(MetaPackIsPackHolder, RejectsOtherShells) {
  EXPECT_FALSE(IsPackHolderV<int>);
  EXPECT_FALSE((IsPackHolderV<std::tuple<int>>));
}

TEST(MetaPackIsPackHolder, StripsCVRef) {
  EXPECT_TRUE((IsPackHolderV<const PackHolder<int>&>));
  EXPECT_TRUE((IsPackHolderV<PackHolder<int>&&>));
  EXPECT_TRUE((IsPackHolderV<volatile PackHolder<int>>));
}

// =============================================================
// pack::TypeAt — sugar over meta::TypeAt<I, PackHolder<Args...>>
// =============================================================

TEST(MetaPackTypeAt, FirstElement) {
  EXPECT_TRUE((std::is_same_v<TypeAtT<0, int, double, char>, int>));
}

TEST(MetaPackTypeAt, MiddleElement) {
  EXPECT_TRUE((std::is_same_v<TypeAtT<1, int, double, char>, double>));
}

TEST(MetaPackTypeAt, LastElement) {
  EXPECT_TRUE((std::is_same_v<TypeAtT<2, int, double, char>, char>));
}

// =============================================================
// pack::Count
// =============================================================

TEST(MetaPackCount, NotPresent) {
  EXPECT_EQ((CountV<float, int, double, char>), 0u);
}

TEST(MetaPackCount, PresentOnce) {
  EXPECT_EQ((CountV<int, int, double, char>), 1u);
}

TEST(MetaPackCount, PresentMultipleTimes) {
  EXPECT_EQ((CountV<int, int, double, int, char, int>), 3u);
}

TEST(MetaPackCount, EmptyPack) { EXPECT_EQ((CountV<int>), 0u); }

// =============================================================
// pack::IsDistinct
// =============================================================

TEST(MetaPackIsDistinct, EmptyIsDistinct) { EXPECT_TRUE((IsDistinctV<>)); }

TEST(MetaPackIsDistinct, AllUnique) {
  EXPECT_TRUE((IsDistinctV<int, double, char>));
}

TEST(MetaPackIsDistinct, Duplicates) {
  EXPECT_FALSE((IsDistinctV<int, int>));
  EXPECT_FALSE((IsDistinctV<int, double, int>));
}

// =============================================================
// pack::IndexOf — constrained to Distinct packs
// =============================================================

TEST(MetaPackIndexOf, FirstElement) {
  EXPECT_EQ((IndexOfV<int, int, double, char>), 0u);
}

TEST(MetaPackIndexOf, MiddleElement) {
  EXPECT_EQ((IndexOfV<double, int, double, char>), 1u);
}

TEST(MetaPackIndexOf, LastElement) {
  EXPECT_EQ((IndexOfV<char, int, double, char>), 2u);
}

}  // namespace
