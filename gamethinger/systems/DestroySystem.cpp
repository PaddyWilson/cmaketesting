#include "DestroySystem.h"

#include "../Components.h"

void DestroySystem::Update(float deltaTime)
{
	auto view = registry->view<DestroyFlag>();
	for (auto [entity, destroy] : view.each())
	{
		registry->destroy(entity);
	}
}
