#include "cuteC2TestScene.h"

#include "../PreFabs.h"

#include "../Vector2Math.h"
#include "../CuteC2Components.h"

#include "../CuteC2System.h"

#include <rlImGui.h>
#include <imgui.h>
#include "../ScriptRegistry.h"

#include "../TestScripts.h"
#include <raymath.h>

void Cute_c2TestScene::Init()
{
	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });

	Position* tempP;
	e = Prefabs::CreateBasicEntity(registry, Vector2(10, 10), "AABB");
	Prefabs::AddAABB(registry, e, Vector2{ 2.5, 2.5 });

	/*for (size_t x = 0; x < 10; x++)
	{
		for (size_t y = 0; y < 10; y++)
		{
			float x1 = ((GRID_CELL_SIZE) * x) + ((GRID_CELL_SIZE) / 2);
			float y1 = ((GRID_CELL_SIZE) * y) + ((GRID_CELL_SIZE) / 2);

			e = Prefabs::CreateBasicEntity(registry, Vector2(x1, y1), "AABB");
			auto t = Prefabs::AddAABB(registry, e, Vector2{ (GRID_CELL_SIZE) / 2, (GRID_CELL_SIZE) / 2 });
			t->debugColor = BLACK;
		}
	}*/

	//tempP = &registry->get<Position>(e);
	//registry->emplace<BB_AABB>(e, tempP->position, Vector2{ 25, 25 });

	e = Prefabs::CreateBasicEntity(registry, Vector2(10, 20), "Circle");
	Prefabs::AddCircle(registry, e, 2.5);
	// tempP = &registry->get<Position>(e);
	//registry->emplace<BB_CIRCLE>(e, tempP->position, 25);

	e = Prefabs::CreateBasicEntity(registry, Vector2(10, 50), "Circle");
	Prefabs::AddCircle(registry, e, 2.5);
	// tempP = &registry->get<Position>(e);
	//registry->emplace<BB_CIRCLE>(e, tempP->position, 25);

	vector<Vector2> cord;
	cord.push_back(Vector2{ -1, -1 });
	cord.push_back(Vector2{ -1, 2 });
	cord.push_back(Vector2{ 1, 1 });
	cord.push_back(Vector2{ 1, -1 });
	e = Prefabs::CreateBasicEntity(registry, Vector2(20, 20), "Poly");
	pos = &registry->get<Position>(e);
	registry->emplace<BB_POLY>(e, pos->position, cord);//, false);

	cord.clear();
	cord.push_back(Vector2{ -2.00,-1.00 });
	cord.push_back(Vector2{ -1.00,-2.00 });
	cord.push_back(Vector2{ 1.00,-2.00 });
	cord.push_back(Vector2{ 2.00,-1.00 });
	cord.push_back(Vector2{ 2.00,1.00 });
	cord.push_back(Vector2{ 1.00,2.00 });
	cord.push_back(Vector2{ -1.00,2.00 });
	cord.push_back(Vector2{ -2.00,1.00 });
	e = Prefabs::CreateBasicEntity(registry, Vector2(20, 20), "Poly");
	pos = &registry->get<Position>(e);
	registry->emplace<BB_POLY>(e, pos->position, cord, false);


	e = Prefabs::CreateBasicEntity(registry, Vector2(40, 40), "Cap");
	//pos = &registry->get<Position>(e);
	//registry->emplace<BB_CAP>(e, pos->position, Vector2{ -1,0 }, Vector2{ 1,0 }, 1.25, false);

	tempP = &registry->get<Position>(e);
	registry->emplace<BB_CAP>(e, tempP->position, Vector2{ -1,0 }, Vector2{ 1,0 }, 1.25, false);
	registry->emplace<ScriptComp>(e).Bind<TestController>();

	//script testing
	e = Prefabs::CreateBasicEntity(registry, Vector2(62, 62), "Circle");
	auto a = Prefabs::AddCircle(registry, e, 2);
	a->debugColor = PURPLE;
	a->isStatic = false;
	ScriptRegistry::AddScriptToEntity("TestController", registry, e);
	//registry->emplace<ScriptComp>(e).Bind<TestController>();

	e = Prefabs::CreateBasicEntity(registry, Vector2(70, 70), "Rectangle");
	registry->get<Position>(e).rotation = 0.f;
	Prefabs::AddRectangle(registry, e, Vector2(2.f, 2.f), false);

	//parent/child testing
	e = Prefabs::CreateBasicEntity(registry, Vector2(55, 60), "Circle Parent");
	a = Prefabs::AddCircle(registry, e, 2);

	/*auto* children = &registry->emplace<Children>(e);
	auto child = Prefabs::CreateBasicEntity(registry, Vector2(60, 60), "Circle Child");
	Prefabs::AddCircle(registry, child, 2.5);
	children->children.push_back(child);

	child = Prefabs::CreateBasicEntity(registry, Vector2(60, 60), "Circle Child");
	Prefabs::AddCircle(registry, child, 2.5);
	children->children.push_back(child);

	child = Prefabs::CreateBasicEntity(registry, Vector2(60, 60), "Circle Child");
	Prefabs::AddCircle(registry, child, 2.5);
	children->children.push_back(child);*/

	//ray.p.x = (GetScreenWidth() / 2) / CUTE_SCALE_FACTOR;
	//ray.p.y = (GetScreenHeight() / 2) / CUTE_SCALE_FACTOR;
}

void Cute_c2TestScene::End() {}


int nextShape = 0;
void Cute_c2TestScene::Update(float deltaTime)
{
	systemManager->Update(deltaTime);

	pos->position = GetMousePosition() / 10;
	/*auto mouse = GetMousePosition();

	mouseAABB.Move(GetMousePosition() / CUTE_SCALE_FACTOR);

	ray.d.x = mouse.x / CUTE_SCALE_FACTOR;
	ray.d.y = mouse.y / CUTE_SCALE_FACTOR;
	ray.t = 50 / CUTE_SCALE_FACTOR;
	*/

	// is imgui being used
	ImGuiIO& io = ImGui::GetIO();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse)
	{
		switch (nextShape)
		{
		case 0:
		{
			auto e = Prefabs::CreateBasicEntity(registry, GetMousePosition() / 10, "Circle");
			auto tempP = &registry->get<Position>(e);
			registry->emplace<BB_CIRCLE>(e, tempP->position, 2.5);
			break;
		}
		case 1:
		{
			auto e = Prefabs::CreateBasicEntity(registry, GetMousePosition() / 10, "AABB");
			auto tempP = &registry->get<Position>(e);
			registry->emplace<BB_AABB>(e, tempP->position, Vector2{ 2.5, 2.5 });
			break;
		}
		case 2:
		{
			auto e = Prefabs::CreateBasicEntity(registry, GetMousePosition() / 10, "Circle");
			auto tempP = &registry->get<Position>(e);
			registry->emplace<BB_CIRCLE>(e, tempP->position, 1.0);
			break;
		}
		case 3:
		{
			auto e = Prefabs::CreateBasicEntity(registry, GetMousePosition() / 10, "AABB");
			auto tempP = &registry->get<Position>(e);
			registry->emplace<BB_AABB>(e, tempP->position, Vector2{ 1.0, 1.0 });
			break;
		}
		case 4:
		{
			auto e = Prefabs::CreateBasicEntity(registry, GetMousePosition() / 10, "Cap");
			auto tempP = &registry->get<Position>(e);
			registry->emplace<BB_CAP>(e, tempP->position, Vector2{ -1,0 }, Vector2{ 1,0 }, 1.25);
			break;
		}
		default:
			break;
		}

		nextShape++;
		if (nextShape > 4)
			nextShape = 0;

		cout << registry->view<BB_CIRCLE>().size() << " " << registry->view<BB_AABB>().size() << " "
			<< registry->view<BB_POLY>().size() << " " << registry->view<BB_CAP>().size() << " "
			<< registry->view<BB_AABB>().size() + registry->view<BB_CIRCLE>().size()
			+ registry->view<BB_POLY>().size() + registry->view<BB_CAP>().size() << endl;
	}
}

//#include "Vector2Math.h"

void Cute_c2TestScene::Render()
{
	systemManager->Render();

	//DrawLine(ray.p.x * CUTE_SCALE_FACTOR, ray.p.y * CUTE_SCALE_FACTOR, ray.d.x * CUTE_SCALE_FACTOR, ray.d.y * CUTE_SCALE_FACTOR, BROWN);

	//for (size_t i = 0; i < shapes.size(); i++)
	//{
	//	shapes[i]->DebugDraw();

	//	c2Raycast cast;

	//	auto mouse = GetMousePosition() / CUTE_SCALE_FACTOR;
	//	c2v mp{ mouse.x, mouse.y };

	//	ray.d = c2Norm(c2Sub(mp, ray.p));
	//	ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

	//	auto hit = c2CastRay(ray, shapes[i]->c2Body, NULL, shapes[i]->type, &cast);
	//	if (hit)
	//	{
	//		ray.t = cast.t;
	//		c2v impact = c2Impact(ray, ray.t);
	//		c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));

	//		DrawRaycast(impact, end);
	//	}
	//	//draws line from origin to point of impact i think
	//	//DrawLine(ray.p.x * CUTE_SCALE_FACTOR, ray.p.y * CUTE_SCALE_FACTOR, (ray.p.x + ray.d.x * ray.t) * CUTE_SCALE_FACTOR, (ray.p.y + ray.d.y * ray.t) * CUTE_SCALE_FACTOR, GREEN);

	//	for (size_t j = i; j < shapes.size(); j++)
	//	{
	//		if (i == j)
	//			continue;

	//		c2Manifold m;

	//		c2Collide(shapes[i]->c2Body, NULL, shapes[i]->type, shapes[j]->c2Body, NULL, shapes[j]->type, &m);
	//		DrawMan(m);
	//	}
	//}
}

void Cute_c2TestScene::RenderUI()
{
	systemManager->RenderUI();
}
