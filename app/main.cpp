// NOLINTBEGIN

#include <iostream>
#include <libkxi/sorted_tuple.hpp>
#include <libkxi/tuple.hpp>
#include <tuple>

int main() {
  kxi::tuple::Tuple<int, float, int> t(1, 2.f, 3);
  kxi::tuple::Tuple<int, float, int> t2(1, 2.f, 3);
  auto t3 = kxi::tuple::TupleCat(t, t2);
  t.Get<1>() = 2.2f;
  std::cout << t.Get<1>() << '\n';
  t3.Get<3>() = 3;

  std::tuple<int, double> ttt;
  std::cout << std::get<1>(ttt) << '\n';
}

// NOLINTEND