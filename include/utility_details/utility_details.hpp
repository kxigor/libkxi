#pragma once

namespace kxi::utility::details {
template <typename T, template <typename...> typename Template>
struct IsSpecOfImpl {
  static constexpr const bool value = false;
};

template <template <typename...> typename Template, typename... Args>
struct IsSpecOfImpl<Template<Args...>, Template> {
  static constexpr const bool value = true;
};
}  // namespace kxi::utility::details