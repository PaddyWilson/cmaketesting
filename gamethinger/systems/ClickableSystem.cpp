#include "ClickableSystem.h"

#include <raymath.h>

void ClickableSystem::Update(float deltaTime)
{
	auto cam = registry->view<Camera2D>();
	for (auto [entity, camera] : cam.each()) {

		//TODO: Action::UP only works when with colliding with shape
		auto screenMouse = GetMousePosition();
		auto worldMouse = GetScreenToWorld2D(screenMouse, camera) / DEFAULT_SCALING_FACTOR;

		LoopBBScreen<BB_AABB>(screenMouse / DEFAULT_SCALING_FACTOR);
		LoopBBScreen<BB_RECTANGLE>(screenMouse / DEFAULT_SCALING_FACTOR);
		LoopBBScreen<BB_CIRCLE>(screenMouse / DEFAULT_SCALING_FACTOR);
		LoopBBScreen<BB_POLY>(screenMouse / DEFAULT_SCALING_FACTOR);
		LoopBBScreen<BB_CAP>(screenMouse / DEFAULT_SCALING_FACTOR);

		LoopBBWorld<BB_AABB>(worldMouse);
		LoopBBWorld<BB_RECTANGLE>(worldMouse);
		LoopBBWorld<BB_CIRCLE>(worldMouse);
		LoopBBWorld<BB_POLY>(worldMouse);
		LoopBBWorld<BB_CAP>(worldMouse);
	}
}

void ClickableSystem::Click(Clickable& click)
{
	if (!click.enabled)
		return;

	bool thisUpdate = IsMouseButtonDown(click.mouseButton);

	Clickable::Action action = Clickable::Action::NONE;
	if (!click.lastUpdate && thisUpdate)
		action = Clickable::Action::PRESSED;
	else if (click.lastUpdate && thisUpdate)//has one frame of lag
		action = Clickable::Action::DOWN;
	else if (click.lastUpdate && !thisUpdate)
		action = Clickable::Action::RELEASED;
	else if (!click.lastUpdate && !thisUpdate)//has one frame of lag
		action = Clickable::Action::UP;

	if (click.action == action && click.func != NULL)
		click.func();

	click.lastUpdate = thisUpdate;
}

bool ClickableSystem::Collision(Vector2 point, BaseShape& shape)
{
	BB_CIRCLE circle(point, 0.01, false);
	circle.Move(point);
	return c2Collided(shape.c2Body, NULL, shape.type, circle.c2Body, NULL, circle.type);
}
