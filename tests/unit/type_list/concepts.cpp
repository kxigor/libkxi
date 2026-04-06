#include <gtest/gtest.h>

#include <libkxi/type_list.hpp>

namespace {

using kxi::type_list::IsTypeListSpecV;
using kxi::type_list::TypeList;

TEST(TypeListConcepts, TypeListSpecIsDetected) {
  EXPECT_TRUE(IsTypeListSpecV<TypeList<>>);
  EXPECT_TRUE(IsTypeListSpecV<TypeList<int>>);
  EXPECT_TRUE((IsTypeListSpecV<TypeList<int, double, char>>));
}

TEST(TypeListConcepts, NonTypeListSpecIsRejected) {
  EXPECT_FALSE(IsTypeListSpecV<int>);
  EXPECT_FALSE(IsTypeListSpecV<double>);
  EXPECT_FALSE(IsTypeListSpecV<void>);
  EXPECT_FALSE((IsTypeListSpecV<std::pair<int, double>>));
}

}  // namespace
