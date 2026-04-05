#pragma once

#include <libkxi/utility_details/core_details.hpp>
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

template <typename T, template <typename...> typename Template>
struct IsSpecOf {
  static constexpr const bool value =
      details::IsSpecOfImpl<std::remove_cvref_t<T>, Template>::value;
};

template <typename T, template <typename...> typename Template>
constexpr const bool IsSpecOfV = IsSpecOf<T, Template>::value;

namespace concepts {
template <typename Self, typename... Args>
concept PerfectCtorGuard =
    (sizeof...(Args) != 1) ||
    (!std::is_same_v<std::remove_cvref_t<Args>, Self> && ...);
}  // namespace concepts

}  // namespace kxi::utility