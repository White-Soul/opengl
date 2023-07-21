#ifndef __TUPLE_HPP__
#define __TUPLE_HPP__
#include "type_traits.hpp"
template <typename... Types>
struct tuple_impl;

template <>
struct tuple_impl<>
{
};

template <typename Head, typename... Tail>
struct tuple_impl<Head, Tail...> : public tuple_impl<Tail...>
{
    using base_type = tuple_impl<Tail...>;
    using value_type = Head;

    value_type _value;

    template<typename FirstV, typename... OtherVs,
    enable_if_t<is_nothrow_constructible_v<value_type, FirstV>, int> = 0>
    explicit tuple_impl(
        FirstV&& value,
        OtherVs&&... others) noexcept 
    : base_type(forward<OtherVs>(others)...), _value(forward<FirstV>(value)){}

    ~tuple_impl() noexcept = default;
};

template<typename... Types>
class tuple : private tuple_impl<Types...>{
    using base_type = tuple_impl<Types...>;
    template<typename ...Values, enable_if_t<sizeof...(Values) == sizeof...(Types), int> = 0>
    explicit tuple(Values&& ... values) noexcept : base_type(forward<Values>(values)...)
    {}
    ~tuple() noexcept = default;

    template<size_t Index, typename... ElemTypes>
    friend constexpr decltype(auto) get(tuple<ElemTypes...>& tuple) noexcept;
};

template<typename Types>
struct tuple_size;
template<typename...Types>
struct tuple_size<tuple<Types...>> : integral_constant<size_t, sizeof...(Types)>
{};

template<typename T>
constexpr size_t tuple_size_v = tuple_size<T>::value;

template<size_t Index, typename Type>
struct tuple_element;

template<size_t Index, typename Type>
struct tuple_element<Index, const Type> : tuple_element<Index, Type>{
    using base = tuple_element<Index, Type>;
    using type = add_const_t<typename base::type>;
};

template<size_t Index>
struct tuple_element<Index, tuple<>>{
    static_assert(always_false<integral_constant<size_t, Index>>, "tuple index out of bounds");
};

template<typename Head, typename... Tail>
struct tuple_element<0,tuple<Head, Tail...>>{
    using type = Head;
    using class_type = tuple<Head, Tail...>;
};

template<size_t Index, typename Head ,typename... Tail>
struct tuple_element<Index, tuple<Head, Tail...>> : tuple_element<Index-1,tuple<Tail...>>
{};

template<size_t Index, typename... Types>
using tuple_element_t = typename tuple_element<Index, Types...>::type;

template<size_t Index, typename... ElemTypes>
constexpr tuple_element_t<Index, tuple<ElemTypes...>>& get(tuple<ElemTypes...>& tuple) noexcept{
    using class_type = typename tuple_element<N, tuple<Types...>>::class_type;
    return static_cast<class_type&>(tuple)._value;
}

template<typename... Types>
constexpr tuple<unrefwrap_t<Types>...> make_tuple(Types&&... args){
    using Ttype = tuple<unrefwrap_t<Types>...>;
    return Ttype(forward<Types>(args)...);
}

template<class...Types>
constexpr tuple<Types&...> tie(Types&... args) noexcept {
    using Ttype = tuple<Types&...>;
    return Ttype(args...);
}

template<class... Types>
constexpr tuple<Types&&...> forward_as_tuple(Types&&... args) noexcept{
    return tuple<Types&&...>(forward<Types>(args)...);
}


#endif