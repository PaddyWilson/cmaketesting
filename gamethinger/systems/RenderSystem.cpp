#include "RenderSystem.h"
#include <raymath.h>
void RenderSystem::Update(float deltaTime)
{
}

void RenderSystem::Render()
{
	auto camera = registry->view<Camera2D>();
	for (auto [entity, camera] : camera.each()) {
		BeginMode2D(camera);

		int bufZone = 100;

		Vector2 points[4];
		points[0] = GetScreenToWorld2D(Vector2{ 0.f - bufZone,0.f - bufZone }, camera) / scalingFactor;//top left
		points[1] = GetScreenToWorld2D(Vector2{ 0.f - bufZone,(float)Global::ScreenHeight + bufZone }, camera) / scalingFactor;//bottom left
		points[2] = GetScreenToWorld2D(Vector2{ (float)Global::ScreenWidth + bufZone,(float)Global::ScreenHeight + bufZone }, camera) / scalingFactor;//bottom right
		points[3] = GetScreenToWorld2D(Vector2{ (float)Global::ScreenWidth + bufZone,0.f - bufZone }, camera) / scalingFactor;//top right

		//over size the screen a little so things on the edge dont disapear

		DrawLineEx(points[0] * scalingFactor, points[1] * scalingFactor, 2, RED);
		DrawLineEx(points[2] * scalingFactor, points[1] * scalingFactor, 2, RED);
		DrawLineEx(points[3] * scalingFactor, points[2] * scalingFactor, 2, RED);
		DrawLineEx(points[3] * scalingFactor, points[0] * scalingFactor, 2, RED);

		//sprite
		auto view = registry->view<Position, SpriteComp, Renderable>();
		for (auto [entity, position, spr, r] : view.each()) {
			if (CheckCollisionPointPoly(position.position, points, 4))
				RenderSprite(position, spr);
		}

		//rectangle
		auto view2 = registry->view<Position, RectangleComp, Renderable>();
		for (auto [entity, position, rec, r] : view2.each()) {
			if (CheckCollisionPointPoly(position.position, points, 4))
				RenderRectangle(position, rec);
		}

		//circle
		auto view3 = registry->view<Position, CircleComp, Renderable>();
		for (auto [entity, position, rec, r] : view3.each()) {
			if (CheckCollisionPointPoly(position.position, points, 4))
				RenderCircle(position, rec);
		}

		//line
		auto view4 = registry->view<Position, LineComp, Renderable>();
		for (auto [entity, position, rec, r] : view4.each()) {
			if (CheckCollisionPointPoly(position.position, points, 4))
				RenderLine(position, rec);
		}
		//text
		auto view5 = registry->view<Position, TextComp, Renderable>();
		for (auto [entity, position, rec, r] : view5.each()) {
			if (CheckCollisionPointPoly(position.position, points, 4))
				RenderText(position, rec);
		}

		EndMode2D();
	}
}

void RenderSystem::RenderUI()
{
	auto view1 = registry->view<Position, SpriteComp, RenderableUI>();
	// use a range-for
	for (auto [entity, position, spr, r] : view1.each()) {
		RenderSprite(position, spr);
	}
	//rectangle
	auto view2 = registry->view<Position, RectangleComp, RenderableUI>();
	for (auto [entity, position, rec, r] : view2.each()) {
		RenderRectangle(position, rec);
	}//circle
	auto view3 = registry->view<Position, CircleComp, RenderableUI>();
	for (auto [entity, position, rec, r] : view3.each()) {
		RenderCircle(position, rec);
	}
	//line
	auto view4 = registry->view<Position, LineComp, RenderableUI>();
	for (auto [entity, position, rec, r] : view4.each()) {
		RenderLine(position, rec);
	}
	//text
	auto view5 = registry->view<Position, TextComp, RenderableUI>();
	for (auto [entity, position, rec, r] : view5.each()) {
		RenderText(position, rec);
	}
}

void RenderSystem::RenderSprite(Position& position, SpriteComp& sprite)
{
	Texture2D tex = TextureManager::GetTexture(sprite.texture);

	//does not have to be the hold texture size
	Rectangle sourceRec{ (float)0,(float)0, (float)tex.width, (float)tex.height };

	//position and size of texture
	Rectangle destRec{
		position.position.x * scalingFactor,
		position.position.y * scalingFactor,
		(sprite.halfSize.x * 2) * scalingFactor,
		(sprite.halfSize.y * 2) * scalingFactor
	};

	DrawTexturePro(tex, sourceRec, destRec, ((position.offset - sprite.halfSize) * scalingFactor) * -1.f, position.rotation, sprite.color);
}

void RenderSystem::RenderRectangle(Position& position, RectangleComp& rectangle)
{
	Rectangle rec = { (position.position.x) * scalingFactor,
		(position.position.y) * scalingFactor,
		(rectangle.halfSize.x * 2) * scalingFactor,
		(rectangle.halfSize.y * 2) * scalingFactor };

	DrawRectanglePro(rec, ((position.offset - rectangle.halfSize) * scalingFactor) * -1.f, position.rotation, rectangle.color);

	//old
	/*rectangle.rec.x = position.position.x;
	rectangle.rec.y = position.position.y;
	DrawRectanglePro(rectangle.rec, position.offset, position.rotation, color.color);*/
}

void RenderSystem::RenderCircle(Position& position, CircleComp& circle)
{
	auto deg = position.rotation / (180 / 3.14159f);

	//i dont like this
	Vector2 p1 = position.position + Vector2Rotate((position.offset), deg);
	DrawCircleV(p1 * scalingFactor, circle.radious * scalingFactor, circle.color);
}

void RenderSystem::RenderLine(Position& position, LineComp& line)
{
	auto deg = position.rotation / (180 / 3.14159);
	Vector2 p1 = Vector2Rotate(line.point1 - position.offset, (float)deg) + position.position;
	Vector2 p2 = Vector2Rotate(line.point2 - position.offset, (float)deg) + position.position;

	if (line.thickness == 1.0f)
		DrawLineV(p1 * scalingFactor, p2 * scalingFactor, line.color);//this is a little faster if thickness is 1;
	else
		DrawLineEx(p1 * scalingFactor, p2 * scalingFactor, line.thickness, line.color);
}

void RenderSystem::RenderText(Position& position, TextComp& text)
{
	Vector2 halfSize = MeasureTextEx(GetFontDefault(), text.text.c_str(), text.size, text.padding) / 2;
	DrawTextPro(GetFontDefault(), text.text.c_str(),
		position.position * scalingFactor,
		((position.offset - halfSize) * scalingFactor) * -1,
		position.rotation,
		text.size * scalingFactor,
		text.padding * scalingFactor,
		text.color);
}

