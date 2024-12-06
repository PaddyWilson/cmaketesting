#pragma once

#include <entt/entt.hpp>

namespace EVENTS
{
	class SerializeOnLoadFinished
	{
	public:
		entt::registry* reg;
	};
}