#include "MovementSystem.h"
#include <raymath.h>
void MovementSystem::Update(float deltaTime)
{
	auto view = registry->view<Position, Velocity>(entt::exclude<Parent>);
	//update parents first
	for (auto [entity, position, velocity] : view.each()) {

		//rotate first then move
		position.rotation += velocity.rotation * deltaTime;

		auto deg = position.rotation / (180 / 3.14159f);
		position.position += Vector2Rotate(velocity.velocity, deg) * deltaTime;

		//add flag to know its been processed
		registry->emplace<WorldPositionUpdated>(entity);
	}

	//update children positions increasing in depth
	//100 might be over kill
	int maxChildDepth = 100;
	for (size_t i = 0; i < maxChildDepth; i++)
	{
		//update children
		auto view2 = registry->view<Position, ChildPosition, Parent>(entt::exclude<WorldPositionUpdated>);
		// use a range-for
		for (auto [entity, position, childPos, parent] : view2.each()) {
			if (parent.depth != i)
				continue;

			//get parent position
			auto p_position = registry->get<Position>(parent.parent);

			auto velocity = registry->try_get<Velocity>(entity);
			if (velocity)
			{
				//move and rotate child position
				childPos.position += velocity->velocity * deltaTime;
				childPos.rotation += velocity->rotation * deltaTime;
			}

			//update real position with parent and child information
			auto p_deg = p_position.rotation / (180 / 3.14159f);
			position.position = p_position.position + Vector2Rotate(childPos.position, p_deg);
			position.rotation = p_position.rotation + childPos.rotation;

			//add flag to know its been processed
			registry->emplace<WorldPositionUpdated>(entity);
		}
	}

	registry->storage<WorldPositionUpdated>().clear();
}
