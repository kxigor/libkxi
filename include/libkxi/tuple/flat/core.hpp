#pragma once

#include <libkxi/types.hpp>
#include <utility>

#include "core_details.hpp"

namespace kxi::tuple::flat {
template <typename... Args>
class FlatTuple
    : public details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                                    meta::Types<Args...>> {
  /*====================== Usings/Helpers ======================*/
  using BaseT =
      details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                             meta::Types<Args...>>;
  using BaseT::BaseT;
};

}  // namespace kxi::tuple::flat