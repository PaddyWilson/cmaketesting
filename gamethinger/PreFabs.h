#pragma once

#include <entt/entt.hpp>
#include <string>
#include <box2d/box2d.h>

#include "TextureManager.h"
#include "Components.h"
#include "CuteC2Components.h"

using namespace entt;
using namespace std;

namespace Prefabs {
	entt::entity CreateBasicEntity(entt::registry* reg, Vector2 position, std::string tag);
	entt::entity CreateBasicEntity(entt::registry* reg, Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0.f);
	entt::entity CreateBasicEntity(entt::registry* reg, Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0.f, std::string tag = "No Tag", uint64_t id = 0);

	entt::entity CreateCamera(entt::registry* reg, Vector2 target, Vector2 offset, float rotation = 0.f, float zoom = 1.0f);

	entt::entity CreateSprite(entt::registry* reg, float x, float y, float size_x, float size_y, string texture);
	entt::entity CreateSpriteCircle(entt::registry* reg, float x, float y, float size_x, float size_y, string texture);
	entt::entity CreateRectangle(entt::registry* reg, float x, float y, float size_x, float size_y, Color color);
	entt::entity CreateCircle(entt::registry* reg, float x, float y, float radious, Color color);
	entt::entity CreateLine(entt::registry* reg, float x, float y, float size_x, float size_y, Color color);
	entt::entity CreateButton(entt::registry* reg, string text, float x, float y, float size, std::function<void()> func, Color boxColor, Color textColor = BLACK);
	entt::entity CreateButtonUI(entt::registry* reg, string text, float x, float y, float size, std::function<void()> func, Color boxColor, Color textColor = BLACK);

	entt::entity CreateBox2dRec(entt::registry* reg, b2World& world, float x, float y, float size_x, float size_y, Rigidbody2D::RigidbodyType dynamic, Color boxColor);
	entt::entity CreateBox2dCircle(entt::registry* reg, b2World& world, float x, float y, float radious, Rigidbody2D::RigidbodyType dynamic, Color color);

	entt::entity CreateLabel(entt::registry* reg, string text, float x, float y, float size, Color textColor);

	BB_AABB* AddAABB(entt::registry* reg, entt::entity ent, Vector2 halfSize, bool isStatic = false);
	BB_RECTANGLE* AddRectangle(entt::registry* reg, entt::entity ent, Vector2 halfSize, bool isStatic = false);
	BB_CIRCLE* AddCircle(entt::registry* reg, entt::entity ent, float radius, bool isStatic = false);
	BB_POLY* AddPoly(entt::registry* reg, entt::entity ent, vector<Vector2> points, bool isStatic = true);
	BB_CAP* AddCap(entt::registry* reg, entt::entity ent, Vector2 p1, Vector2 p2, float radius, bool isStatic = true);

}