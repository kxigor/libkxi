#pragma once

#include <libkxi/tuple/core_details.hpp>
#include <libkxi/types.hpp>
#include <utility>

namespace kxi::tuple {
template <typename... Args>
class FlatTuple
    : public details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                                    het::Types<Args...>> {
  /*====================== Usings/Helpers ======================*/
  using BaseT =
      details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                             het::Types<Args...>>;
  using BaseT::BaseT;
};

}  // namespace kxi::tuple