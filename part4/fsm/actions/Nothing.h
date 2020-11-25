#pragma once

struct Nothing
{
	template <typename Machine, typename State, typename Event>
	void execute(Machine&, State&, const Event&)
	{
	}
};

static constexpr auto stringify(Types<Nothing>) { return StaticString{"Nothing"}; }
