#include <gtest/gtest.h>

#include <libkxi/tuple.hpp>
#include <libkxi/type_list.hpp>
#include <type_traits>

namespace {

using kxi::tuple::Tuple;
using kxi::type_list::TupleAsTypeListT;
using kxi::type_list::TypeList;
using kxi::type_list::TypeListAsTupleT;

// --- TupleAsTypeList ---

TEST(TypeListTupleOps, TupleToTypeList) {
  using Result = TupleAsTypeListT<Tuple<int, double, char>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double, char>>));
}

TEST(TypeListTupleOps, EmptyTupleToTypeList) {
  using Result = TupleAsTypeListT<Tuple<>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<>>));
}

TEST(TypeListTupleOps, ConstTupleToTypeList) {
  using Result = TupleAsTypeListT<const Tuple<int, double>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListTupleOps, LvalueRefTupleToTypeList) {
  using Result = TupleAsTypeListT<Tuple<int, double>&>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListTupleOps, ConstLvalueRefTupleToTypeList) {
  using Result = TupleAsTypeListT<const Tuple<int, double>&>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListTupleOps, RvalueRefTupleToTypeList) {
  using Result = TupleAsTypeListT<Tuple<int, double>&&>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListTupleOps, ConstRvalueRefTupleToTypeList) {
  using Result = TupleAsTypeListT<const Tuple<int, double>&&>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, double>>));
}

TEST(TypeListTupleOps, VolatileTupleToTypeList) {
  using Result = TupleAsTypeListT<volatile Tuple<int, char>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, char>>));
}

TEST(TypeListTupleOps, ConstVolatileTupleToTypeList) {
  using Result = TupleAsTypeListT<const volatile Tuple<int, char>>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, char>>));
}

TEST(TypeListTupleOps, ConstVolatileRefTupleToTypeList) {
  using Result = TupleAsTypeListT<const volatile Tuple<int, char>&>;
  EXPECT_TRUE((std::is_same_v<Result, TypeList<int, char>>));
}

// --- TypeListAsTuple ---

TEST(TypeListTupleOps, TypeListToTuple) {
  using Result = TypeListAsTupleT<TypeList<int, double, char>>;
  EXPECT_TRUE((std::is_same_v<Result, Tuple<int, double, char>>));
}

TEST(TypeListTupleOps, EmptyTypeListToTuple) {
  using Result = TypeListAsTupleT<TypeList<>>;
  EXPECT_TRUE((std::is_same_v<Result, Tuple<>>));
}

TEST(TypeListTupleOps, SingleElementTypeListToTuple) {
  using Result = TypeListAsTupleT<TypeList<float>>;
  EXPECT_TRUE((std::is_same_v<Result, Tuple<float>>));
}

// --- Round-trip ---

TEST(TypeListTupleOps, RoundTripTupleToListAndBack) {
  using Original = Tuple<int, float, char>;
  using RoundTrip = TypeListAsTupleT<TupleAsTypeListT<Original>>;
  EXPECT_TRUE((std::is_same_v<RoundTrip, Original>));
}

TEST(TypeListTupleOps, RoundTripListToTupleAndBack) {
  using Original = TypeList<long, short, double>;
  using RoundTrip = TupleAsTypeListT<TypeListAsTupleT<Original>>;
  EXPECT_TRUE((std::is_same_v<RoundTrip, Original>));
}

TEST(TypeListTupleOps, RoundTripFromQualifiedTuple) {
  using Original = Tuple<int, float>;
  using RoundTrip =
      TypeListAsTupleT<TupleAsTypeListT<const volatile Original&&>>;
  EXPECT_TRUE((std::is_same_v<RoundTrip, Original>));
}

}  // namespace
