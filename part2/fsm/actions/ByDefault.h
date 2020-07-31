#pragma once

template <typename Action>
struct ByDefault
{
	template <typename Event>
	Action handle(const Event&) const
	{
		return Action{};
	}
};
