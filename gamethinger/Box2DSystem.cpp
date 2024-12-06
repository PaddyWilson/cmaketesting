#include "Box2DSystem.h"

#include "Components.h"

void Box2DSystem::Init()
{
}

void Box2DSystem::Reset()
{

}

void Box2DSystem::Update(float deltaTime)
{
	auto view = registry->view<Position, Rigidbody2D>();
	for (auto [entity, position, rigidbody] : view.each()) {

		if (rigidbody.bodyId == b2_nullBodyId)
			continue;

		auto rPos = b2Body_GetPosition(rigidbody.bodyId);// rigidbody.body->GetPosition();
		position.position.x = rPos.x;
		position.position.y = rPos.y;
		position.rotation = b2Rot_GetAngle(b2Body_GetRotation(rigidbody.bodyId));//->GetAngle() * RAD2DEG;
	}
}

void Box2DSystem::Render()
{
	/*auto view = registry->view<Rigidbody2D, Rigidbody2DRectangle, ColorComp>();
	for (auto [entity, rigid, rec, color] : view.each()) {
		if (rec.fixture != nullptr && rigid.body != nullptr)
		{
			DrawRectanglePro(
				Rectangle{ rigid.body->GetPosition().x,rigid.body->GetPosition().y, rec.halfSize.x * 2, rec.halfSize.y * 2 },
				rec.halfSize, rigid.body->GetAngle() * RAD2DEG, color.color);
		}
	}

	auto view2 = registry->view<Rigidbody2D, Rigidbody2DCircle, ColorComp>();
	for (auto [entity, rigid, circle, color] : view2.each()) {
		if (circle.fixture != nullptr && rigid.body != nullptr)
		{
			DrawCircleV(Vector2(rigid.body->GetPosition().x, rigid.body->GetPosition().y), circle.radius, color.color);
		}
	}*/
}

void Box2DSystem::OnRigidbody2DAdd()
{
}

void Box2DSystem::OnRigidbody2DRemove()
{
}
