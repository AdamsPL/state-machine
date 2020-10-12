#pragma once

#include <tuple>
#include <variant>

template <typename... States>
class StateMachine
{
public:
    StateMachine() = default;

    StateMachine(States &&... states)
        : states(std::forward<States>(states)...)
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
    std::tuple<States...> states;
    std::variant<States*...> currentState{ &std::get<0>(states) };
};
