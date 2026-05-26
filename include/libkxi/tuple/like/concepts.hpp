#pragma once

#include <concepts>
#include <cstddef>
#include <libkxi/meta.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple::like::concepts {

namespace details {
template <typename T, typename Seq>
inline constexpr bool HasAllIndexGetsV = false;

template <typename T, std::size_t... Is>
inline constexpr bool HasAllIndexGetsV<T, std::index_sequence<Is...>> =
    requires(T tuple) { (tuple.template Get<Is>(), ...); };

template <typename T, typename TypeList>
inline constexpr bool HasAllTypeGetsV = false;

template <typename T, typename... Types>
inline constexpr bool HasAllTypeGetsV<T, meta::pack::PackHolder<Types...>> =
    requires(T tuple) { (tuple.template Get<Types>(), ...); };
}  // namespace details

template <typename T>
concept TupleLike = requires(T tuple) {
  { meta::SizeV<T> } -> std::convertible_to<std::size_t>;
} && details::HasAllIndexGetsV<T, std::make_index_sequence<meta::SizeV<T>>>;

template <typename T>
concept SwappableTuple =
    TupleLike<T> && requires(T tuple) { tuple.Swap(tuple); };

template <typename T>
concept DistinctTupleLike = TupleLike<T> && requires {
  typename meta::RebindT<T, meta::pack::PackHolder>;
} && details::HasAllTypeGetsV<T, meta::RebindT<T, meta::pack::PackHolder>>;

}  // namespace kxi::tuple::like::concepts