#pragma once

#include <type_traits>

namespace kxi::tuple_like::concepts {

template <typename T>
concept TupleLike = requires(std::remove_cvref_t<T> t) {
  t.template Get<0>();
  t.Swap(t);
};

}  // namespace kxi::tuple_like::concepts