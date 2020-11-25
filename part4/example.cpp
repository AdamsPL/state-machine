#include <fsm/types/Types.h>
#include <fsm/actions/utils.h>
#include <fsm/types/utils.h>
#include <fsm/utils/StaticString.h>

#include "Door.h"

#include <iostream>
#include <type_traits>

template <typename T>
void debug(T&&)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

#define STRINGIFY_IMPL(TYPE) [[maybe_unused]] static constexpr auto stringify(Types<TYPE>) { return StaticString{#TYPE}; }

STRINGIFY_IMPL(OpenEvent)
STRINGIFY_IMPL(CloseEvent)
STRINGIFY_IMPL(LockEvent)
STRINGIFY_IMPL(UnlockEvent)

STRINGIFY_IMPL(ClosedState)
STRINGIFY_IMPL(OpenState)
STRINGIFY_IMPL(LockedState)

struct Header {};

struct SimpleStringifier
{
    constexpr auto operator()(Types<Header>) const
    {
        return StaticString{""};
    }

    template <typename T>
    constexpr auto operator()(Types<T> type) const
    {
        return stringify(type);
    }
};

template <std::size_t Width>
struct ConstantWidthStringifier
{
    constexpr auto operator()(Types<Header>) const
    {
        return StaticString{""}.template changeLength<Width>(' ');
    }

    template <typename T>
    constexpr auto operator()(Types<T> type) const
    {
        return stringify(type).template changeLength<Width>(' ');
    }
};

template <typename Stringifier, typename State>
class GenerateRow
{
public:
    constexpr GenerateRow(Stringifier str, Types<State>)
        : str(str)
    {
    }

    constexpr auto operator()(Types<State> state) const
    {
        return str(state);
    }

    template <typename Event>
    constexpr auto operator()(Types<Event>) const
    {
        auto action = ResolveAction{}(Types<Types<State, Event>>{});
        return StaticString{" | "} + str(action);
    }

private:
    const Stringifier str;
};

template <typename Stringifier>
class GenerateRow<Stringifier, Header>
{
public:
    constexpr GenerateRow(Stringifier str, Types<Header>)
        : str(str)
    {
    }

    constexpr auto operator()(Types<Header> header) const
    {
        return str(header);
    }

    template <typename Event>
    constexpr auto operator()(Types<Event> event) const
    {
        return StaticString{" | "} + str(event);
    }

private:
    const Stringifier str;
};

template <typename Stringifier, typename... Events>
class GenerateTable
{
public:
    constexpr GenerateTable(Stringifier str, Types<Events...>)
        : str(str)
    {
    }

    template <typename State>
    constexpr auto operator()(Types<State> state) const
    {
        return (Types<State, Events...>{} | MapAndJoin{GenerateRow{str, state}}) + StaticString{"\n"};
    }

private:
    const Stringifier str;
};

template <std::size_t X>
struct Maximum
{
    template <std::size_t Y>
    constexpr auto operator+(Maximum<Y>) const
    {
        return Maximum<std::max(X, Y)>{};
    }

    static constexpr auto value() const
    {
        return X;
    }
};

struct CalculateMaxLength
{
    template <typename T>
    constexpr auto operator()(Types<T> type)
    {
        return Maximum<stringify(type).length()>{};
    }
};

template <typename... StateTypes, typename... EventTypes>
constexpr auto generateTransitionTable(Types<StateTypes...> states, Types<EventTypes...> events)
{
    constexpr SimpleStringifier stringifier;
    constexpr auto result = (Types<Header>{} + states) | MapAndJoin{GenerateTable{stringifier, events}};
    return result;
}

template <typename... StateTypes, typename... EventTypes>
constexpr auto generatePrettyTransitionTable(Types<StateTypes...> states, Types<EventTypes...> events)
{
    constexpr auto actions = (states * events) | MapAndJoin(ResolveAction{});
    constexpr auto maxWidth = (states + events + actions) | MapAndJoin(CalculateMaxLength{});
    constexpr ConstantWidthStringifier<maxWidth.value()> stringifier{};
    constexpr auto result = (Types<Header>{} + states) | MapAndJoin{GenerateTable{stringifier, events}};
    return result;
}

int main()
{
    std::cout << generateTransitionTable(Door::getStateTypes(), Types<OpenEvent, CloseEvent, LockEvent, UnlockEvent>{}).data() << std::endl;
    std::cout << generatePrettyTransitionTable(Door::getStateTypes(), Types<OpenEvent, CloseEvent, LockEvent, UnlockEvent>{}).data() << std::endl;
    return 0;
}
