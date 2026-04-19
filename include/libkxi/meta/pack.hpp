#pragma once

#include <cstddef>
#include <type_traits>

#include "core.hpp"

namespace kxi::meta::pack {

template <typename... Args>
struct PackHolder {};

namespace details {
template <typename T>
struct IsPackHolderImpl : std::false_type {};

template <typename... Types>
struct IsPackHolderImpl<PackHolder<Types...>> : std::true_type {};
}  // namespace details

template <typename T>
using IsPackHolder = details::IsPackHolderImpl<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr auto IsPackHolderV = IsPackHolder<T>::value;

namespace concepts {
template <typename T>
concept PackHolder = IsPackHolderV<T>;
}  // namespace concepts

template <std::size_t I, typename... Types>
using TypeAt = meta::TypeAt<I, PackHolder<Types...>>;

template <std::size_t I, typename... Types>
using TypeAtT = typename TypeAt<I, Types...>::type;

template <typename T, typename... Types>
using Count = meta::Count<T, PackHolder<Types...>>;

template <typename T, typename... Types>
inline constexpr std::size_t CountV = Count<T, Types...>::value;

template <typename... Types>
using IsDistinct = meta::IsDistinct<PackHolder<Types...>>;

template <typename... Types>
inline constexpr auto IsDistinctV = IsDistinct<Types...>::value;

namespace concepts {
template <typename... Types>
concept Distinct = IsDistinctV<Types...>;
}  // namespace concepts

template <typename T, typename... Types>
requires concepts::Distinct<Types...>
using IndexOf = meta::IndexOf<T, PackHolder<Types...>>;

template <typename T, typename... Types>
requires concepts::Distinct<Types...>
inline constexpr const std::size_t IndexOfV = IndexOf<T, Types...>::value;

};  // namespace kxi::meta::pack