#pragma once

template <typename... Handlers>
struct Will : Handlers...
{
	using Handlers::handle...;
};

