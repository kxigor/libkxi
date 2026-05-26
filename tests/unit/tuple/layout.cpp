#include <gtest/gtest.h>

#include <cstdint>
#include <libkxi/tuple/flat.hpp>

namespace {

using kxi::tuple::flat::Tuple;

struct Empty {};
struct EmptyAlso {};

TEST(TupleLayout, EmptyTupleIsMinimal) { EXPECT_EQ(sizeof(Tuple<>), 1u); }

TEST(TupleLayout, SingleEmptyTypeOccupiesOneByte) {
  EXPECT_EQ(sizeof(Tuple<Empty>), 1u);
}

TEST(TupleLayout, TwoEmptyTypesCollapseToOne) {
  // [[no_unique_address]] on IndexedType::value_ should let two empty
  // bases share storage, keeping the tuple at the 1-byte minimum.
  EXPECT_EQ((sizeof(Tuple<Empty, EmptyAlso>)), 1u);
}

TEST(TupleLayout, EmptyDoesNotInflateNonEmpty) {
  EXPECT_EQ((sizeof(Tuple<int, Empty>)), sizeof(int));
  EXPECT_EQ((sizeof(Tuple<Empty, int>)), sizeof(int));
}

TEST(TupleLayout, SingleIntMatchesIntSize) {
  EXPECT_EQ(sizeof(Tuple<int>), sizeof(int));
}

struct PaddedProbe {
  std::int8_t a;
  std::int32_t b;
  std::int8_t c;
};

TEST(TupleLayout, PaddingReflectsDeclarationOrder) {
  // flat::Tuple keeps types in declaration order, so layout follows the same
  // padding rules as PaddedProbe. This pins the contract — and lets
  // compact::CompactTuple be measured against it.
  EXPECT_EQ((sizeof(Tuple<std::int8_t, std::int32_t, std::int8_t>)),
            sizeof(PaddedProbe));
}

}  // namespace
