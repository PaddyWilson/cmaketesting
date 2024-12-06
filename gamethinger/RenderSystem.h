#pragma once
#include <raylib.h>
#include <entt/entt.hpp>

#include "TextureManager.h"
#include "BaseSystem.h"
#include "Components.h"
#include "Vector2Math.h"

class RenderSystem :public BaseSystem
{
public:
	RenderSystem() { systemName = "RenderSystem"; }
	void Update(float deltaTime);
	void Render();
	void RenderUI();

	void SetScalingFactor(float scale) {
		scalingFactor = scale;
	};
	float GetScalingFactor() {
		return scalingFactor;
	};

private:
	float scalingFactor = DEFAULT_SCALING_FACTOR;

	void RenderSprite(Position& position, SpriteComp& sprite);
	void RenderRectangle(Position& position, RectangleComp& rectangle);
	void RenderCircle(Position& position, CircleComp& circle);
	void RenderLine(Position& position, LineComp& circle);
	void RenderText(Position& position, TextComp& text);
};

