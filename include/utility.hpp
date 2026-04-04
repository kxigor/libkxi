#pragma once

#include <type_traits>

namespace kxi::utility {
template <typename From, typename To>
struct CopyConst {
  using type = To;
};

template <typename From, typename To>
struct CopyConst<const From, To> {
  using type = const To;
};

template <typename From, typename To>
using CopyConstT = typename CopyConst<From, To>::type;

namespace concepts {
template <typename Self, typename... Args>
concept PerfectCtorGuard =
    (sizeof...(Args) != 1) ||
    (!std::is_same_v<std::remove_cvref_t<Args>, Self> && ...);
}

}  // namespace kxi::utility