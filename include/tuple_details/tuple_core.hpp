#pragma once

#include <utility>

#include "../type_list.hpp"
#include "tuple_core_details.hpp"

namespace kxi::flat_tuple {
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

}  // namespace kxi::flat_tuple