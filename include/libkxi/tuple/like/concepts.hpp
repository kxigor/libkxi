#pragma once

#include <libkxi/meta.hpp>
#include <libkxi/types.hpp>
#include <type_traits>

namespace kxi::tuple::like::concepts {

namespace details {

template <typename T, typename Seq>
constexpr bool has_all_index_gets_v = false;

template <typename T, std::size_t... Is>
constexpr bool has_all_index_gets_v<T, std::index_sequence<Is...>> =
    requires(T tuple) { (tuple.template Get<Is>(), ...); };

template <typename T, typename TypeList>
constexpr bool has_all_type_gets_v = false;

template <typename T, typename... Types>
constexpr bool has_all_type_gets_v<T, meta::Types<Types...>> =
    requires(T tuple) { (tuple.template Get<Types>(), ...); };

}  // namespace details

template <typename T>
concept TupleLike = requires(T tuple) {
  { meta::SizeV<std::remove_cvref_t<T>> } -> std::convertible_to<std::size_t>;
} && details::has_all_index_gets_v<T, std::make_index_sequence<meta::SizeV<T>>>;

template <typename T>
concept SwappableTuple =
    TupleLike<T> &&
    requires(std::remove_cvref_t<T> tuple) { tuple.Swap(tuple); };

template <typename T>
concept DistinctTupleLike = TupleLike<T> && requires {
  typename meta::RebindT<std::remove_cvref_t<T>, meta::Types>;
} && details::has_all_type_gets_v<T, meta::RebindT<T, meta::Types>>;

}  // namespace kxi::tuple::like::concepts