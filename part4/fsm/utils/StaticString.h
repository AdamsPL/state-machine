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
        return join(resize<N-1>(chars, '\0'), rhs.chars);
    }

    constexpr bool operator==(const StaticString<N> &rhs) const
    {
        return areEqual(chars, rhs.chars);
    }

    constexpr std::size_t length() const
    {
        return N - 1;
    }

    template <std::size_t TargetLen>
    constexpr StaticString<TargetLen + 1> changeLength(char fill) const
    {
        constexpr std::array<const char, 1> stringEnd{'\0'};
        return join(resize<TargetLen>(resize<N-1>(chars, fill), fill), stringEnd);
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

[[maybe_unused]] constexpr void testLength()
{
    constexpr StaticString lhs{"abc"};
    constexpr size_t expected{3};
    static_assert(lhs.length() == expected);
}

[[maybe_unused]] constexpr void test0Length()
{
    constexpr StaticString lhs{""};
    constexpr size_t expected{0};
    static_assert(lhs.length() == expected);
}

[[maybe_unused]] constexpr void testChangeLength()
{
    constexpr StaticString shorter{"abc"};
    constexpr StaticString longer{"abcdef"};
    constexpr StaticString empty{""};

    constexpr size_t minLength{5};
    constexpr StaticString expectedShorter{"abcxx"};
    constexpr StaticString expectedLonger {"abcde"};
    constexpr StaticString expectedEmpty  {"zzzzz"};

    constexpr auto res = shorter.changeLength<minLength>('x');

    static_assert(res.data()[3] == expectedShorter.data()[3]);

    static_assert(shorter.changeLength<minLength>('x') == expectedShorter);
    static_assert(longer.changeLength<minLength>('y') == expectedLonger);
    static_assert(empty.changeLength<minLength>('z') == expectedEmpty);
}

}
}
