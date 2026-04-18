#pragma once

#include <array>
#include <cstddef>
#include <libkxi/meta/concepts.hpp>
#include <libkxi/traits.hpp>
#include <type_traits>

namespace kxi::meta {

namespace details {
template <typename TList>
struct SizeImpl;

template <template <typename...> typename Shell, typename... Types>
struct SizeImpl<Shell<Types...>> : traits::SizeConstant<sizeof...(Types)> {};
}  // namespace details

template <concepts::VariadicTemplate TList>
using Size = details::SizeImpl<std::remove_cvref_t<TList>>;

template <concepts::VariadicTemplate TList>
inline constexpr std::size_t SizeV = Size<TList>::value;

namespace details {
template <std::size_t I, typename TList>
struct TypeAtImpl;

template <template <typename...> typename Shell, std::size_t I, typename Head,
          typename... Tail>
struct TypeAtImpl<I, Shell<Head, Tail...>> {
  using type = TypeAtImpl<I - 1, Shell<Tail...>>::type;
};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct TypeAtImpl<0, Shell<Head, Tail...>> {
  using type = Head;
};
}  // namespace details

template <std::size_t I, concepts::VariadicTemplate TList>
using TypeAt = details::TypeAtImpl<I, std::remove_cvref_t<TList>>;

template <std::size_t I, concepts::VariadicTemplate TList>
using TypeAtT = typename TypeAt<I, TList>::type;

namespace details {
template <typename T, typename TList>
struct CountImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct CountImpl<T, Shell<Head, Tail...>>
    : traits::SizeConstant<CountImpl<T, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct CountImpl<Head, Shell<Head, Tail...>>
    : traits::SizeConstant<1 + CountImpl<Head, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head>
struct CountImpl<Head, Shell<>> : traits::SizeConstant<0> {};
}  // namespace details

template <typename T, concepts::VariadicTemplate TList>
using Count = details::CountImpl<T, std::remove_cvref_t<TList>>;

template <typename T, concepts::VariadicTemplate TList>
inline constexpr std::size_t CountV = Count<T, TList>::value;

namespace details {
template <typename TList>
struct IsDistinctImpl : std::true_type {};

template <template <typename...> typename Shell, typename... Types>
struct IsDistinctImpl<Shell<Types...>> {
  using TList = Shell<Types...>;
  static constexpr bool value = ((CountV<Types, TList> == 1) && ...);
};
};  // namespace details

template <concepts::VariadicTemplate TList>
using IsDistinct = details::IsDistinctImpl<std::remove_cvref_t<TList>>;

template <concepts::VariadicTemplate TList>
inline constexpr auto IsDistinctV = IsDistinct<TList>::value;

namespace concepts {
template <typename TList>
concept Distinct = IsDistinctV<TList>;
};  // namespace concepts

namespace details {
template <typename T, typename TList>
struct IndexOfImpl;

template <template <typename...> typename Shell, typename T, typename Head,
          typename... Tail>
struct IndexOfImpl<T, Shell<Head, Tail...>>
    : traits::SizeConstant<1 + IndexOfImpl<T, Shell<Tail...>>::value> {};

template <template <typename...> typename Shell, typename Head,
          typename... Tail>
struct IndexOfImpl<Head, Shell<Head, Tail...>> : traits::SizeConstant<0> {};
}  // namespace details

template <typename T, concepts::Distinct TList>
using IndexOf = details::IndexOfImpl<T, std::remove_cvref_t<TList>>;

template <typename T, concepts::Distinct TList>
inline constexpr const std::size_t IndexOfV = IndexOf<T, TList>::value;

namespace details {
template <std::size_t I, typename... TLists>
struct LocateIndexImpl {
 public:
  struct IndexLocation {
    std::size_t list_pos{};
    std::size_t elem_pos{};
  };

 private:
  static constexpr IndexLocation Get() {
    static constexpr std::array kListSizes = {Size<TLists>::value...};
    std::size_t current_list = 0;
    std::size_t current_pos = I;

    for (const auto& list_size : kListSizes) {
      if (current_pos < list_size) {
        break;
      }
      current_list += 1;
      current_pos -= list_size;
    }

    return {.list_pos = current_list, .elem_pos = current_pos};
  }

 public:
  static constexpr const IndexLocation value = Get();
};
}  // namespace details

template <std::size_t I, concepts::VariadicTemplate... TLists>
using LocateIndex = details::LocateIndexImpl<I, std::remove_cvref_t<TLists>...>;

template <std::size_t I, concepts::VariadicTemplate... TLists>
inline constexpr auto LocateIndexV = LocateIndex<I, TLists...>::value;

namespace details {
template <typename... TLists>
struct CatListsImpl;

template <template <typename...> typename Shell, typename... CurrTypes,
          typename... NextTypes, typename... NextTLists>
struct CatListsImpl<Shell<CurrTypes...>, Shell<NextTypes...>, NextTLists...> {
  using type =
      CatListsImpl<Shell<CurrTypes..., NextTypes...>, NextTLists...>::type;
};

template <template <typename...> typename Shell, typename... CurrTypes>
struct CatListsImpl<Shell<CurrTypes...>> {
  using type = Shell<CurrTypes...>;
};
}  // namespace details

template <concepts::VariadicTemplate... TLists>
using CatLists = details::CatListsImpl<std::remove_cvref_t<TLists>...>;

template <concepts::VariadicTemplate... TLists>
using CatListsT = CatLists<TLists...>::type;

namespace details {
template <typename TList, template <typename...> typename Shell>
struct RebindImpl;

template <template <typename...> typename OldShell,
          template <typename...> typename Shell, typename... Types>
struct RebindImpl<OldShell<Types...>, Shell> {
  using type = Shell<Types...>;
};
}  // namespace details

template <concepts::VariadicTemplate TList,
          template <typename...> typename Shell>
using Rebind = details::RebindImpl<std::remove_cvref_t<TList>, Shell>;

template <typename TList, template <typename...> typename Shell>
using RebindT = Rebind<TList, Shell>::type;

template <template <typename...> typename Template>
struct TemplateHolder {
  template <typename... Types>
  using type = Template<Types...>;
};

namespace details {
template <typename TList>
struct UnbindImpl;

template <template <typename...> typename Shell, typename... Types>
struct UnbindImpl<Shell<Types...>> {
  using type = TemplateHolder<Shell>;
};
};  // namespace details

template <concepts::VariadicTemplate TList>
using Unbind = details::UnbindImpl<TList>;

template <concepts::VariadicTemplate TList>
using UnbindT = Unbind<TList>::type;

template <typename... Types>
struct IsAllSameTypes : std::true_type {};

template <typename T, typename... Types>
struct IsAllSameTypes<T, Types...>
    : std::integral_constant<bool, (std::is_same_v<T, Types> && ...)> {};

template <typename... Types>
inline constexpr auto IsAllSameTypesV = IsAllSameTypes<Types...>::value;

template <template <typename...> typename FirstTemplate,
          template <typename...> typename SecondTemplate>
struct IsSameTemplate : std::is_same<TemplateHolder<FirstTemplate>,
                                     TemplateHolder<SecondTemplate>> {};

template <template <typename...> typename FirstTemplate,
          template <typename...> typename SecondTemplate>
inline constexpr auto IsSameTemplateV =
    IsSameTemplate<FirstTemplate, SecondTemplate>::value;

template <template <typename...> typename... Templates>
struct IsAllSameTemplates : IsAllSameTypes<TemplateHolder<Templates...>> {};

template <template <typename...> typename... Templates>
inline constexpr auto IsAllSameTemplatesV =
    IsAllSameTemplates<Templates...>::value;

template <concepts::VariadicTemplate FirstTList,
          concepts::VariadicTemplate SecondTList>
struct IsSameShell : std::is_same<UnbindT<FirstTList>, UnbindT<SecondTList>> {};

template <concepts::VariadicTemplate FirstTList,
          concepts::VariadicTemplate SecondTList>
inline constexpr auto IsSameShellV =
    IsSameShell<FirstTList, SecondTList>::value;

template <concepts::VariadicTemplate... TLists>
struct IsAllSameShells : IsAllSameTypes<UnbindT<TLists>...> {};

template <concepts::VariadicTemplate... TLists>
inline constexpr auto IsAllSameShellsV = IsAllSameShells<TLists...>::value;

namespace concepts {
template <typename... Types>
concept AllSameShells = IsAllSameShellsV<Types...>;
}  // namespace concepts

namespace details {
template <typename... TList>
struct UnbindSamePackImpl;

template <template <typename...> typename Shell, typename... Types,
          typename... OtherTLists>
struct UnbindSamePackImpl<Shell<Types...>, OtherTLists...> {
  using type = TemplateHolder<Shell>;
};
}  // namespace details

template <concepts::VariadicTemplate... TLists>
requires concepts::AllSameShells<TLists...>
using UnbindSamePack =
    details::UnbindSamePackImpl<std::remove_cvref_t<TLists>...>;

template <concepts::VariadicTemplate... TLists>
requires concepts::AllSameShells<TLists...>
using UnbindSamePackT = UnbindSamePack<TLists...>::type;

};  // namespace kxi::meta