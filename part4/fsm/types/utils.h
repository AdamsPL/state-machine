#pragma once

#include <fsm/types/Types.h>

template <typename Operation>
class MapAndJoin
{
public:
    constexpr MapAndJoin(Operation operation)
        : operation(operation)
    {
    }

    template <typename... Ts>
    constexpr auto operator()(Types<Ts>... rhs)
    {
        return (operation(rhs) + ...);
    }

private:
    Operation operation;
};
