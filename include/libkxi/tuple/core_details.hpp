#include <cstddef>
#include <libkxi/heterogeneous.hpp>
#include <libkxi/types.hpp>
#include <libkxi/utility.hpp>
#include <type_traits>
#include <utility>

namespace kxi::tuple::details {

template <utility::concepts::IndexSequence IndexesCortage,
          het::concepts::Types TypesCortage>

struct FlatTupleImpl;

template <std::size_t... Indexes, typename... Types>
class FlatTupleImpl<std::index_sequence<Indexes...>, het::Types<Types...>>
    : utility::IndexedType<Indexes, Types>... {
  /*====================== Usings/Helpers ======================*/
 private:
  using IndexesCortage = std::index_sequence<Indexes...>;
  using TypesCortage = het::Types<Types...>;

  /*================= Constructors/Destructors =================*/
 public:
  constexpr FlatTupleImpl() = default;

  constexpr FlatTupleImpl(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl(FlatTupleImpl&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<FlatTupleImpl, Args...>
  constexpr FlatTupleImpl(Args&&... args)
      : utility::IndexedType<Indexes, Types>(std::forward<Args>(args))... {}

  constexpr ~FlatTupleImpl() = default;

  /*======================= Assignments ========================*/
  constexpr FlatTupleImpl& operator=(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl& operator=(FlatTupleImpl&& /*unused*/) = default;

  /*======================== Interface =========================*/
  template <std::size_t I, typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    using BaseT = kxi::utility::CopyCVT<Self, utility::IndexedType<I, T>>;

    return std::forward_like<Self>(static_cast<BaseT&>(self).Value());
  }

  template <std::size_t I, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<I, het::GetTypeT<het::Types, I, TypesCortage>>();
  }

  template <typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<het::GetIndexV<het::Types, T, TypesCortage>>();
  }

  constexpr void Swap(FlatTupleImpl& other) noexcept(
      (std::is_nothrow_swappable_v<Types> && ...)) {
    (std::swap(Get<Indexes>(), other.Get<Indexes>()), ...);
  }
};

}  // namespace kxi::tuple::details