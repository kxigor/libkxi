// NOLINTBEGIN

#include <iostream>
#include <tuple.hpp>

int main() {
  kxi::flat_tuple::Tuple<int, float, int> t(1, 2.f, 3);
  kxi::flat_tuple::Tuple<int, float, int> t2(1, 2.f, 3);
  auto t3 = kxi::flat_tuple::TupleCat(t, t2);
  t.Get<1>() = 2.2f;
  std::cout << t.Get<1>() << '\n';
  t3.Get<3>() = 3;
}

// NOLINTEND