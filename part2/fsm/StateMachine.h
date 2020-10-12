#pragma once

#include <tuple>
#include <variant>

template <typename... _States>
class States;

template <typename... _Events>
class Events;

template <typename States, typename Events>
class StateMachine;

namespace details {
    template<class... B>
        using or_ = std::disjunction<B...>;

    template <typename T, typename Tuple>
        struct has_type;

    template <typename T, typename... Us>
        struct has_type<T, std::tuple<Us...>> : or_<std::is_same<T, Us>...> {};
}

template <typename... _States, typename... _Events>
class StateMachine<States<_States...>, Events<_Events...>>
{
    using events = std::tuple<_Events...>;
public:
    StateMachine() = default;

    StateMachine(_States &&... states)
        : states(std::forward<_States>(states)...)
    {
    }

    template <typename State>
    State& transitionTo()
    {
        State& state = std::get<State>(states);
        currentState = &state;
        return state;
    }

    template <typename Event>
    void handle(const Event& event)
    {
        static_assert(details::has_type<Event, events>::value, "Unhandled event type");
        handleBy(event, *this);
    }

    template <typename Event>
    void handleBy(const Event& event, StateMachine& machine)
    {
        auto passEventToState = [&machine, &event] (auto statePtr) {
            auto action = statePtr->handle(event);
            action.execute(machine, *statePtr, event);
        };
        std::visit(passEventToState, currentState);
    }

private:
    std::tuple<_States...> states;
    std::variant<_States*...> currentState{ &std::get<0>(states) };
};
