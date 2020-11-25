#pragma once

#include <cstddef>

template <typename... Ts>
struct Types
{
};

template <typename... Lhs, typename... Rhs>
constexpr auto operator+(Types<Lhs...>, Types<Rhs...>)
{
    return Types<Lhs..., Rhs...>{};
}

template <typename Lhs, typename... Rhs>
constexpr auto operator*(Types<Lhs>, Types<Rhs...>)
{
    return Types<Types<Lhs, Rhs>...>{};
}

template <typename... Lhs, typename Rhs>
constexpr auto operator*(Types<Lhs...>, Rhs rhs)
{
    return ((Types<Lhs>{} * rhs) + ...);
}

template <typename... Ts, typename Operation>
constexpr auto operator|(Types<Ts...>, Operation oper)
{
    return oper(Types<Ts>{}...);
}

template <typename... Ts>
constexpr std::size_t size(Types<Ts...>)
{
    return sizeof...(Ts);
}
