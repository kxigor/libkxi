#pragma once

#include <libkxi/meta.hpp>  // IWYU pragma: keep
#include <utility>

#include "core_details.hpp"

namespace kxi::tuple::flat {
template <typename... Args>
class Tuple
    : public details::TupleImpl<std::make_index_sequence<sizeof...(Args)>,
                                meta::pack::PackHolder<Args...>> {
  /*====================== Usings/Helpers ======================*/
  using BaseT = details::TupleImpl<std::make_index_sequence<sizeof...(Args)>,
                                   meta::pack::PackHolder<Args...>>;
  using BaseT::BaseT;
};

}  // namespace kxi::tuple::flat