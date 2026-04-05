#include <cstddef>
#include <type_traits>
#include <utility>

#include "../type_list.hpp"
#include "../utility.hpp"

namespace kxi::tuple::details {

template <std::size_t I, typename T>
struct FlatTypeHolder {
 public:
  /*===================== Usings/Constants =====================*/
  static constexpr std::size_t kHolderNumber = I;
  using ValueT = T;

  /*================= Constructors/Destructors =================*/
  constexpr FlatTypeHolder() = default;

  constexpr FlatTypeHolder(const FlatTypeHolder& /*unused*/) = default;

  constexpr FlatTypeHolder(FlatTypeHolder&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<FlatTypeHolder, Args...> &&
           std::is_constructible_v<ValueT, Args...>
  constexpr FlatTypeHolder(Args&&... args)
      : value_(std::forward<Args>(args)...) {}

  constexpr ~FlatTypeHolder() = default;

  /*======================= Assignments ========================*/
  constexpr FlatTypeHolder& operator=(const FlatTypeHolder& /*unused*/) =
      default;

  constexpr FlatTypeHolder& operator=(FlatTypeHolder&& /*unused*/) = default;

  /*========================= Getters ==========================*/
  ValueT& Value() { return value_; }

  const ValueT& Value() const { return value_; }

 private:
  /*======================= Data fields ========================*/
  ValueT value_;
};

template <typename IndexesCortage, typename TypesCortage>
struct FlatTupleImpl;

template <std::size_t... Indexes, typename... Types>
class FlatTupleImpl<std::index_sequence<Indexes...>,
                    type_list::TypeList<Types...>>
    : FlatTypeHolder<Indexes, Types>... {
  /*====================== Usings/Helpers ======================*/
 private:
  using IndexesCortage = std::index_sequence<Indexes...>;

  using TypesCortage = type_list::TypeList<Types...>;

  /*================= Constructors/Destructors =================*/
 public:
  constexpr FlatTupleImpl() = default;

  constexpr FlatTupleImpl(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl(FlatTupleImpl&& /*unused*/) = default;

  template <typename... Args>
  requires utility::concepts::PerfectCtorGuard<FlatTupleImpl, Args...>
  constexpr FlatTupleImpl(Args&&... args)
      : FlatTypeHolder<Indexes, Types>(std::forward<Args>(args))... {}

  constexpr ~FlatTupleImpl() = default;

  /*======================= Assignments ========================*/
  constexpr FlatTupleImpl& operator=(const FlatTupleImpl& /*unused*/) = default;

  constexpr FlatTupleImpl& operator=(FlatTupleImpl&& /*unused*/) = default;

  /*======================== Interface =========================*/
  template <std::size_t I, typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    using BaseT = kxi::utility::CopyConstT<std::remove_reference_t<Self>,
                                           FlatTypeHolder<I, T>>;

    return std::forward_like<Self>(static_cast<BaseT&>(self).Value());
  }

  template <std::size_t I, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<I, type_list::GetTypeT<TypesCortage, I>>();
  }

  template <typename T, typename Self>
  constexpr decltype(auto) Get(this Self&& self) {
    return std::forward<Self>(self)
        .template Get<type_list::GetIndexV<TypesCortage, T>>();
  }

  constexpr void Swap(FlatTupleImpl& other) noexcept((std::is_nothrow_swappable_v<Types> && ...)) {
    (std::swap(Get<Indexes>(), other.Get<Indexes>()), ...);
  }

};

}  // namespace kxi::tuple::details