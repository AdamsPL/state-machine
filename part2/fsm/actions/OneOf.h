#pragma once

#include <utility>
#include <variant>

template <typename... Actions>
class OneOf
{
public:
	template <typename T>
	OneOf(T&& arg)
		: options(std::forward<T>(arg))
	{
	}

	template <typename Machine, typename State, typename Event>
	void execute(Machine& machine, State& state, const Event& event)
	{
		std::visit([&machine, &state, &event](auto& action){ action.execute(machine, state, event); }, options);
	}

private:
	std::variant<Actions...> options;
};
