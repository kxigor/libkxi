#include <gtest/gtest.h>

#include <libkxi/meta.hpp>
#include <tuple>
#include <type_traits>

namespace {

using kxi::meta::CommonShellOfT;
using kxi::meta::IsAllSameShellsV;
using kxi::meta::IsAllSameTypesV;
using kxi::meta::IsSameShellV;
using kxi::meta::IsSameTemplateV;
using kxi::meta::RebindT;
using kxi::meta::ShellOfT;
using kxi::meta::TemplateHolder;

template <typename...>
struct TList {};

template <typename...>
struct OtherList {};

// =============================================================
// Rebind
// =============================================================

TEST(MetaRebind, RebindsToDifferentShell) {
  EXPECT_TRUE((std::is_same_v<RebindT<TList<int, double>, std::tuple>,
                              std::tuple<int, double>>));
}

TEST(MetaRebind, RebindsEmptyList) {
  EXPECT_TRUE((std::is_same_v<RebindT<TList<>, std::tuple>, std::tuple<>>));
}

TEST(MetaRebind, RebindsToSameShellIsIdentity) {
  EXPECT_TRUE(
      (std::is_same_v<RebindT<TList<int, double>, TList>, TList<int, double>>));
}

TEST(MetaRebind, StripsCVRefFromInput) {
  EXPECT_TRUE((
      std::is_same_v<RebindT<const TList<int>&, std::tuple>, std::tuple<int>>));
}

// =============================================================
// ShellOf — produces a TemplateHolder for the source shell
// =============================================================

TEST(MetaShellOf, ProducesTemplateHolder) {
  using H = ShellOfT<TList<int>>;
  using Reapplied = typename H::template type<float, double>;
  EXPECT_TRUE((std::is_same_v<Reapplied, TList<float, double>>));
}

TEST(MetaShellOf, EqualForSameShellDifferentArgs) {
  EXPECT_TRUE((std::is_same_v<ShellOfT<TList<int>>, ShellOfT<TList<char>>>));
}

TEST(MetaShellOf, DifferentForDifferentShells) {
  EXPECT_FALSE(
      (std::is_same_v<ShellOfT<TList<int>>, ShellOfT<std::tuple<int>>>));
  EXPECT_FALSE(
      (std::is_same_v<ShellOfT<TList<int>>, ShellOfT<OtherList<int>>>));
}

TEST(MetaShellOf, StripsCVRef) {
  EXPECT_TRUE(
      (std::is_same_v<ShellOfT<TList<int>>, ShellOfT<const TList<int>&>>));
}

// =============================================================
// IsAllSameTypes
// =============================================================

TEST(MetaIsAllSameTypes, EmptyPack) { EXPECT_TRUE((IsAllSameTypesV<>)); }

TEST(MetaIsAllSameTypes, SingleType) { EXPECT_TRUE((IsAllSameTypesV<int>)); }

TEST(MetaIsAllSameTypes, AllSame) {
  EXPECT_TRUE((IsAllSameTypesV<int, int>));
  EXPECT_TRUE((IsAllSameTypesV<int, int, int, int>));
}

TEST(MetaIsAllSameTypes, AnyDifferent) {
  EXPECT_FALSE((IsAllSameTypesV<int, double>));
  EXPECT_FALSE((IsAllSameTypesV<int, int, double>));
  EXPECT_FALSE((IsAllSameTypesV<int, double, int>));
}

TEST(MetaIsAllSameTypes, RespectsCVQualification) {
  EXPECT_FALSE((IsAllSameTypesV<int, const int>));
  EXPECT_FALSE((IsAllSameTypesV<int, int&>));
}

// =============================================================
// IsSameTemplate
// =============================================================

TEST(MetaIsSameTemplate, SameTemplate) {
  EXPECT_TRUE((IsSameTemplateV<TList, TList>));
  EXPECT_TRUE((IsSameTemplateV<std::tuple, std::tuple>));
}

TEST(MetaIsSameTemplate, DifferentTemplates) {
  EXPECT_FALSE((IsSameTemplateV<TList, OtherList>));
  EXPECT_FALSE((IsSameTemplateV<TList, std::tuple>));
}

// =============================================================
// IsAllSameTemplates
// =============================================================

TEST(MetaIsAllSameTemplates, SingleTemplate) {
  EXPECT_TRUE((kxi::meta::IsAllSameTemplatesV<TList>));
}

TEST(MetaIsAllSameTemplates, SameTwoTemplates) {
  EXPECT_TRUE((kxi::meta::IsAllSameTemplatesV<TList, TList>));
}

TEST(MetaIsAllSameTemplates, DifferentTemplates) {
  EXPECT_FALSE((kxi::meta::IsAllSameTemplatesV<TList, OtherList>));
}

TEST(MetaIsAllSameTemplates, ThreeMixed) {
  EXPECT_TRUE((kxi::meta::IsAllSameTemplatesV<TList, TList, TList>));
  EXPECT_FALSE((kxi::meta::IsAllSameTemplatesV<TList, TList, OtherList>));
}

TEST(MetaIsAllSameTemplates, StdTemplates) {
  EXPECT_TRUE((kxi::meta::IsAllSameTemplatesV<std::tuple, std::tuple>));
  EXPECT_FALSE((kxi::meta::IsAllSameTemplatesV<std::tuple, TList>));
}

// =============================================================
// IsSameShell — works on instances, normalises CVref
// =============================================================

TEST(MetaIsSameShell, SameShellWithDifferentArgs) {
  EXPECT_TRUE((IsSameShellV<TList<int>, TList<double, char>>));
  EXPECT_TRUE((IsSameShellV<TList<>, TList<int>>));
}

TEST(MetaIsSameShell, DifferentShells) {
  EXPECT_FALSE((IsSameShellV<TList<int>, OtherList<int>>));
  EXPECT_FALSE((IsSameShellV<TList<int>, std::tuple<int>>));
}

TEST(MetaIsSameShell, StripsCVRef) {
  EXPECT_TRUE((IsSameShellV<const TList<int>&, TList<double>&&>));
}

// =============================================================
// CommonShellOf — requires all shells to match
// =============================================================

TEST(MetaCommonShellOf, ReturnsCommonHolder) {
  using H = CommonShellOfT<TList<int>, TList<double, char>>;
  using Reapplied = typename H::template type<float>;
  EXPECT_TRUE((std::is_same_v<Reapplied, TList<float>>));
}

TEST(MetaCommonShellOf, SingleListIsHolderOfThatShell) {
  using H = CommonShellOfT<TList<int, double>>;
  using Reapplied = typename H::template type<char>;
  EXPECT_TRUE((std::is_same_v<Reapplied, TList<char>>));
}

TEST(MetaCommonShellOf, StripsCVRef) {
  using H = CommonShellOfT<const TList<int>&, TList<double>&&>;
  using Reapplied = typename H::template type<>;
  EXPECT_TRUE((std::is_same_v<Reapplied, TList<>>));
}

// =============================================================
// CatLists — extra edge cases not in core.cpp
// =============================================================

TEST(MetaCatLists, StripsCVRef) {
  using kxi::meta::CatListsT;
  EXPECT_TRUE((std::is_same_v<CatListsT<const TList<int>&, TList<double>&&>,
                              TList<int, double>>));
}

}  // namespace
