#pragma once

template <typename Event, typename Action>
struct On
{
	Action handle(const Event&) const
	{
		return Action{};
	}
};

