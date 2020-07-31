#pragma once

#include <fsm/utils/arrayUtils.h>

#include <string>
#include <cstdlib>
#include <iostream>

template <std::size_t N>
class StaticString
{
public:
    constexpr StaticString(const char (&chars)[N])
        : chars(toStdArray(chars))
    {
    }

    constexpr StaticString(const std::array<const char, N>& chars)
        : chars(chars)
    {
    }

    template <std::size_t M>
    constexpr StaticString<N + M - 1> operator+(const StaticString<M> &rhs) const
    {
        return join(resize<N-1>(chars), rhs.chars);
    }

    constexpr bool operator==(const StaticString<N> &rhs) const
    {
        return areEqual(chars, rhs.chars);
    }

    template <std::size_t M>
    friend class StaticString;

    constexpr const char* data() const
    {
        return chars.data();
    }

private:
    std::array<const char, N> chars;
};

namespace tests
{
namespace
{

[[maybe_unused]] constexpr void testAdding()
{
    constexpr StaticString lhs{"abc"};
    constexpr StaticString rhs{"de"};
    constexpr StaticString expected{"abcde"};
    static_assert(expected == lhs + rhs);
}

}
}
