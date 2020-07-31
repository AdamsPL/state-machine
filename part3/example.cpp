#include <fsm/utils/StaticString.h>
#include <cstdio>

__attribute__((noinline)) void test()
{
    constexpr StaticString first{"<"};
    constexpr StaticString second{"hello"};
    constexpr StaticString third{">"};
    constexpr StaticString result = first + second + third;
    puts(result.data());
}

int main()
{
    test();
    return 0;
}
