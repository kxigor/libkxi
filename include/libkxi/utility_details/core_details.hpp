#pragma once

namespace kxi::utility::details {
template <typename T, template <typename...> typename Template>
struct IsTypeSpecOfImpl {
  static constexpr const bool value = false;
};

template <template <typename...> typename Template, typename... Args>
struct IsTypeSpecOfImpl<Template<Args...>, Template> {
  static constexpr const bool value = true;
};

template<typename T, template<auto...> typename Template>
struct IsValueSpecOfImpl {
  static constexpr const bool value = false;
};

template<template<auto...> typename Template, auto ... Args>
struct IsValueSpecOfImpl<Template<Args...>, Template> {
  static constexpr const bool value = true;
};

}  // namespace kxi::utility::details