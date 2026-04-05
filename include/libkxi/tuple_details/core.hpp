#pragma once

#include <libkxi/tuple_details/core_details.hpp>
#include <libkxi/type_list.hpp>
#include <utility>

namespace kxi::tuple {
template <typename... Args>
class Tuple
    : public details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                                    type_list::TypeList<Args...>> {
  /*====================== Usings/Helpers ======================*/
  using BaseT =
      details::FlatTupleImpl<std::make_index_sequence<sizeof...(Args)>,
                             type_list::TypeList<Args...>>;
  using BaseT::BaseT;
};

}  // namespace kxi::tuple