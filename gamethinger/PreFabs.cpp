#include "PreFabs.h"

#include <raymath.h>

#define Vector2D_Zero Vector2d{0.f,0.f}

entt::entity Prefabs::CreateBasicEntity(entt::registry* reg, Vector2 position, std::string tag)
{
	return CreateBasicEntity(reg, position, Vector2{ 0.f,0.f }, 0, tag, 0);
}

entt::entity Prefabs::CreateBasicEntity(entt::registry* reg, Vector2 position, Vector2 offset, float rotation)
{
	return CreateBasicEntity(reg, position, offset, rotation, "No Tag", 0);
}

entt::entity Prefabs::CreateBasicEntity(entt::registry* reg, Vector2 position, Vector2 offset, float rotation, std::string tag, uint64_t id)
{
	//create entitys
	auto entity = reg->create();

	if (id == 0)
		reg->emplace<UniqueID>(entity);
	else
		reg->emplace<UniqueID>(entity, id);

	reg->emplace<Tag>(entity, tag);
	reg->emplace<Position>(entity, position, offset, rotation);
	return entity;
}

entt::entity Prefabs::CreateCamera(entt::registry* reg, Vector2 postion, Vector2 offset, float rotation, float zoom)
{
	auto entity = CreateBasicEntity(reg, postion, offset, rotation, "Camera2D", 0);
	reg->emplace<Camera2D>(entity, offset, postion, rotation, zoom);
	return entity;
}

entt::entity Prefabs::CreateSprite(entt::registry* reg, float x, float y, float size_x, float size_y, string texture)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Sprite", 0);
	reg->emplace<Velocity>(entity, 0.f, 0.f, 0.f);
	AddRectangle(reg, entity, Vector2{ size_x / 2, size_y / 2 }, false);
	//reg->emplace<>(entity, size_x, size_y);

	SpriteComp* sprite = &reg->emplace<SpriteComp>(entity, texture);
	sprite->halfSize = Vector2{ size_x / 2, size_y / 2 };
	sprite->color = WHITE;

	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateSpriteCircle(entt::registry* reg, float x, float y, float size_x, float size_y, string texture)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "SpriteC", 0);

	reg->emplace<Velocity>(entity, 0.f, 0.f, 0.f);
	//AddRectangle(reg, entity, Vector2(size_x / 2, size_y / 2), false);
	AddCircle(reg, entity, size_y / 2, false);

	SpriteComp* sprite = &reg->emplace<SpriteComp>(entity, texture);
	sprite->halfSize = Vector2{ size_x / 2, size_y / 2 };
	sprite->color = WHITE;
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateRectangle(entt::registry* reg, float x, float y, float size_x, float size_y, Color color)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Rectangle", 0);
	reg->emplace<Velocity>(entity, 0.f, 0.f, 0.f);
	AddRectangle(reg, entity, Vector2{ size_x / 2, size_y / 2 }, false);
	reg->emplace<RectangleComp>(entity, Vector2{ size_x / 2, size_y / 2 }, color);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateCircle(entt::registry* reg, float x, float y, float radious, Color color)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Circle", 0);
	reg->emplace<Velocity>(entity, 0.f, 0.f, 0.f);
	AddCircle(reg, entity, radious / 2, false);
	reg->emplace<CircleComp>(entity, radious, color);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateLine(entt::registry* reg, float x, float y, float size_x, float size_y, Color color)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ size_x / 2, size_y / 2 }, 0.f, "Line", 0);
	reg->emplace<Velocity>(entity, 0.f, 0.f, 0.f);
	reg->emplace<LineComp>(entity, Vector2{ x, y }, Vector2{ size_x, size_y }, 1.f, color);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateButton(entt::registry* reg, string text, float x, float y, float size, std::function<void()> func, Color color, Color textColor)
{
	float padding = 0.1;
	//get text scaled to size and lenght
	auto s = MeasureTextEx(GetFontDefault(), text.c_str(), (float)size, padding) / 2;

	//create entitys
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Button", 0);

	reg->emplace<RectangleComp>(entity, Vector2{ s.x, s.y }, color);
	AddRectangle(reg, entity, Vector2{ s.x, s.y }, false);
	reg->emplace<TextComp>(entity, text, size, padding, textColor);
	reg->emplace<Clickable>(entity, Clickable::Action::PRESSED, func);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateButtonUI(entt::registry* reg, string text, float x, float y, float size, std::function<void()> func, Color color, Color textColor)
{
	//get text scaled to size and lenght
	auto s = MeasureTextEx(GetFontDefault(), text.c_str(), (float)size, 0.1) / 2;

	//create entitys
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Button", 0);

	reg->emplace<RectangleComp>(entity, Vector2{ s.x, s.y }, color);
	AddRectangle(reg, entity, Vector2{ s.x, s.y }, false);
	reg->emplace<TextComp>(entity, text, size, 0.1f, textColor);
	reg->emplace<Clickable>(entity, Clickable::Action::PRESSED, func);
	reg->emplace<RenderableUI>(entity);
	return entity;
}

entt::entity Prefabs::CreateBox2dRec(entt::registry* reg, b2WorldId world, float x, float y, float halfX, float halfY, Rigidbody2D::RigidbodyType dynamic, Color color)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Box2D Rec", 0);
	auto& pos = reg->get<Position>(entity);

	auto& rigid = reg->emplace<Rigidbody2D>(entity, dynamic);
	b2BodyId body = rigid.Init(world, pos);
	auto& box = reg->emplace<Rigidbody2DRectangle>(entity, Vector2{ halfX, halfY });
	box.Init(body);

	reg->emplace<RectangleComp>(entity, Vector2{ halfX, halfY }, color);// , boxColor);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateBox2dCircle(entt::registry* reg, b2WorldId world, float x, float y, float radious, Rigidbody2D::RigidbodyType dynamic, Color color)
{
	auto entity = CreateBasicEntity(reg, Vector2{ x, y }, Vector2{ 0.f, 0.f }, 0.f, "Box2D Circle", 0);
	auto& pos = reg->get<Position>(entity);

	auto& rigid = reg->emplace<Rigidbody2D>(entity, dynamic);
	b2BodyId body = rigid.Init(world, pos);
	auto& box = reg->emplace<Rigidbody2DCircle>(entity, radious);
	box.Init(body);

	reg->emplace<CircleComp>(entity, radious, color);// , boxColor);
	reg->emplace<Renderable>(entity);
	return entity;
}

entt::entity Prefabs::CreateLabel(entt::registry* reg, string text, float x, float y, float size, Color color)
{
	//create entitys
	auto entity = reg->create();

	auto s = MeasureTextEx(GetFontDefault(), text.c_str(), (float)size, .1f) / 2;

	reg->emplace<Position>(entity, x, y, s, 0.f);
	reg->emplace<TextComp>(entity, text, size, .1f, color);
	reg->emplace<Renderable>(entity);
	return entity;
}

BB_AABB* Prefabs::AddAABB(entt::registry* reg, entt::entity ent, Vector2 halfSize, bool isStatic)
{
	auto p = &reg->get<Position>(ent);
	return &reg->emplace<BB_AABB>(ent, p->position, halfSize, isStatic);
}

BB_RECTANGLE* Prefabs::AddRectangle(entt::registry* reg, entt::entity ent, Vector2 halfSize, bool isStatic)
{
	auto p = &reg->get<Position>(ent);
	return &reg->emplace<BB_RECTANGLE>(ent, p->position, halfSize, isStatic);
}

BB_CIRCLE* Prefabs::AddCircle(entt::registry* reg, entt::entity ent, float radius, bool isStatic)
{
	auto p = &reg->get<Position>(ent);
	return &reg->emplace<BB_CIRCLE>(ent, p->position, radius, isStatic);
}

BB_POLY* Prefabs::AddPoly(entt::registry* reg, entt::entity ent, vector<Vector2> points, bool isStatic)
{
	auto p = &reg->get<Position>(ent);
	return &reg->emplace<BB_POLY>(ent, p->position, points, isStatic);
}

BB_CAP* Prefabs::AddCap(entt::registry* reg, entt::entity ent, Vector2 p1, Vector2 p2, float radius, bool isStatic)
{
	auto p = &reg->get<Position>(ent);
	return &reg->emplace<BB_CAP>(ent, p->position, p1, p2, radius, isStatic);
}
