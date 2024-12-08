#pragma once
#include <raylib.h>

#include "../BaseSystem.h"
#include "../Components.h"
#include "../CuteC2Components.h"

class ClickableSystem :  public BaseSystem
{
public:
	ClickableSystem() { systemName = "Clickable"; }
	void Update(float deltaTime);

private:
	void Click(Clickable& click);

	bool Collision(Vector2 point, BaseShape& shape);

	template<typename T>
	bool LoopBBWorld(Vector2 point)
	{
		static_assert(std::is_base_of<BaseShape, T>::value, "T must derive from Base");
		auto view = registry->view<T, Clickable>(entt::exclude<RenderableUI>);
		for (auto [entity, bb, click] : view.each())
		{
			if (Collision(point, bb))
			{
				Click(click);
				return true; // only click one thing
			}
		}
		return false;
	};
	
	template<typename T>
	bool LoopBBScreen(Vector2 point)
	{
		static_assert(std::is_base_of<BaseShape, T>::value, "T must derive from Base");
		auto view = registry->view<T, Clickable, RenderableUI>();
		for (auto [entity, bb, click, render] : view.each())
		{
			if (Collision(point, bb))
			{
				Click(click);
				return true; // only click one thing
			}
		}
		return false;
	};
};

