#include "Camera2DSystem.h"

#include "../Components.h"

void Camera2DSystem::Update(float deltaTime)
{
	auto view = registry->view<Position, Camera2D>();
	for (auto [entity, position, camera] : view.each()) {
		camera.target = position.position;
		camera.offset = position.offset;
		camera.rotation = position.rotation;
		//camera.zoom = zoom; change zoom somewhere else
	}
}
