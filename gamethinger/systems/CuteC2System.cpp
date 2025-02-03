#include "CuteC2System.h"

#include "../EventManager.h"

#include "../CuteMath.h"

#include <iostream>

#include "../TheadPool.h"

CuteC2System::CuteC2System() {
	systemName = "CuteC2System";
}

void CuteC2System::Init()
{
	registry->on_construct<BB_AABB>().connect<&CuteC2System::OnComponentAdd<BB_AABB>>(this);
	registry->on_destroy<BB_AABB>().connect<&CuteC2System::OnComponentRemove<BB_AABB>>(this);

	registry->on_construct<BB_CIRCLE>().connect<&CuteC2System::OnComponentAdd<BB_CIRCLE>>(this);
	registry->on_destroy<BB_CIRCLE>().connect<&CuteC2System::OnComponentRemove<BB_CIRCLE>>(this);

	registry->on_construct<BB_POLY>().connect<&CuteC2System::OnComponentAdd<BB_POLY>>(this);
	registry->on_destroy<BB_POLY>().connect<&CuteC2System::OnComponentRemove<BB_POLY>>(this);

	registry->on_construct<BB_CAP>().connect<&CuteC2System::OnComponentAdd<BB_CAP>>(this);
	registry->on_destroy<BB_CAP>().connect<&CuteC2System::OnComponentRemove<BB_CAP>>(this);

	registry->on_construct<BB_RECTANGLE>().connect<&CuteC2System::OnComponentAdd<BB_RECTANGLE>>(this);
	registry->on_destroy<BB_RECTANGLE>().connect<&CuteC2System::OnComponentRemove<BB_RECTANGLE>>(this);

	//EventManager::AddListener<CollisionPair>([&](CollisionPair c) { /*std::cout << c.shape1->isStatic << std::endl;*/ });
}

void CuteC2System::Reset()
{
	dynamicShapes.clear();
	manafolds.clear();
	collisionPairs.clear();
	staticShapes.clear();
	staticShapesAll.clear();
}

void CuteC2System::Update(float deltaTime)
{
	dynamicShapesMap.clear();
	dynamicShapes.clear();
	manafolds.clear();
	collisionPairs.clear();

	ThreadPool* threads = ThreadPool::GetInstance();

	//should really have two one for static objects and one for moving
	//because static objects don't matter if they collide with eachother
	//std::unordered_map<std::pair<int, int>, std::vector<BaseShape*>, pair_hash> shapeMap;

	/*std::vector<std::future<void>> results;

	results.push_back(threads->AddTask(&CuteC2System::ProcessDynamics<BB_AABB>, this));
	results.push_back(threads->AddTask(&CuteC2System::ProcessDynamics<BB_CIRCLE>, this));
	results.push_back(threads->AddTask(&CuteC2System::ProcessDynamics<BB_POLY>, this));
	results.push_back(threads->AddTask(&CuteC2System::ProcessDynamics<BB_CAP>, this));
	results.push_back(threads->AddTask(&CuteC2System::ProcessDynamics<BB_RECTANGLE>, this));

	for (size_t i = 0; i < results.size(); i++)
	{
		results[i].wait();
	}*/

	ProcessDynamics<BB_AABB>();
	ProcessDynamics<BB_CIRCLE>();
	ProcessDynamics<BB_POLY>();
	ProcessDynamics<BB_CAP>();
	ProcessDynamics<BB_RECTANGLE>();

	//std::vector<std::future<void>> results;

	// find hits
	c2Manifold m;
	for (size_t i = 0; i < dynamicShapes.size(); i++)
	{
		BaseShape* dShape = dynamicShapes[i];
		//results.push_back(threads->AddTask([&](BaseShape* dShape) {

			//loop all static spatial maping
		Vector2 pos = dShape->GetPostion();
		int xm = (int)(pos.x / GRID_CELL_SIZE);//fmod(cellSize, pos.x);
		int ym = (int)(pos.x / GRID_CELL_SIZE);//fmod(cellSize, pos.y);

		for (int x = xm - GRID_SEARCH_RADIUS; x <= xm + GRID_SEARCH_RADIUS; x++)
		{
			for (int y = ym - GRID_SEARCH_RADIUS; y <= ym + GRID_SEARCH_RADIUS; y++)
			{
				std::pair<int, int>key{ x, y };

				auto iter = staticShapes.find(key);
				if (iter == staticShapes.end())
					continue;

				auto& vec = staticShapes.at(key);// [key] ;
				int index = vec.size();
				BaseShape* sShape = NULL;
				for (size_t j = 0; j < index; j++)
				{
					sShape = vec[j];

					if (c2Collided(dShape->c2Body, NULL, dShape->type, sShape->c2Body, NULL, sShape->type))
					{
						c2Collide(dShape->c2Body, NULL, dShape->type, sShape->c2Body, NULL, sShape->type, &m);
						manafolds.push_back(m);
						collisionPairs.push_back(CollisionPair{ dShape->entity, sShape->entity });
					}
				}
			}
		}

		//*************

		//for (int x = xm - GRID_SEARCH_RADIUS; x <= xm + GRID_SEARCH_RADIUS; x++)
		//{
		//	for (int y = ym - GRID_SEARCH_RADIUS; y <= ym + GRID_SEARCH_RADIUS; y++)
		//	{
		//		std::pair<int, int>key{ x, y };

		//		auto iter = dynamicShapesMap.find(key);
		//		if (iter == dynamicShapesMap.end())
		//			continue;

		//		auto& vec = dynamicShapesMap.at(key);// [key] ;
		//		int index = vec.size();
		//		BaseShape* sShape = NULL;
		//		for (size_t j = 0; j < index; j++)
		//		{
		//			sShape = vec[j];

		//			if (sShape->entity == dShape->entity)
		//				continue;

		//			if (c2Collided(dShape->c2Body, NULL, dShape->type, sShape->c2Body, NULL, sShape->type))
		//			{
		//				c2Collide(dShape->c2Body, NULL, dShape->type, sShape->c2Body, NULL, sShape->type, &m);
		//				manafolds.push_back(m);
		//				collisionPairs.push_back(CollisionPair{ dShape->entity, sShape->entity });
		//			}
		//		}
		//	}
		//}

		//loop all dynamic
		for (size_t j = i + 1; j < dynamicShapes.size(); j++)
		{
			if (c2Collided(dShape->c2Body, NULL, dShape->type, dynamicShapes[j]->c2Body, NULL, dynamicShapes[j]->type))
			{
				c2Collide(dShape->c2Body, NULL, dShape->type, dynamicShapes[j]->c2Body, NULL, dynamicShapes[j]->type, &m);
				manafolds.push_back(m);
				collisionPairs.push_back(CollisionPair{ dShape->entity, dynamicShapes[j]->entity });
			}
		}

		//}, dynamicShapes[i]));

	//****

	//loop all with a raycast
	//shapes[i]->DebugDraw();
	///*
	//c2Raycast cast;

	//auto mouse = GetMousePosition() / CUTE_SCALE_FACTOR;
	//c2v mp{ mouse.x, mouse.y };

	//ray.d = c2Norm(c2Sub(mp, ray.p));
	//ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

	//auto hit = c2CastRay(ray, shapes[i]->c2Body, NULL, shapes[i]->type, &cast);
	//if (hit)
	//{
	//	ray.t = cast.t;
	//	c2v impact = c2Impact(ray, ray.t);
	//	c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));

	//	DrawRaycast(impact, end);
	//}*/
	////draws line from origin to point of impact i think
	////DrawLine(ray.p.x * CUTE_SCALE_FACTOR, ray.p.y * CUTE_SCALE_FACTOR, (ray.p.x + ray.d.x * ray.t) * CUTE_SCALE_FACTOR, (ray.p.y + ray.d.y * ray.t) * CUTE_SCALE_FACTOR, GREEN);

	//for (size_t j = i + 1; j < shapes.size(); j++)
	//{
	//	if (c2Collided(shapes[i]->c2Body, NULL, shapes[i]->type, shapes[j]->c2Body, NULL, shapes[j]->type))
	//	{
	//		c2Collide(shapes[i]->c2Body, NULL, shapes[i]->type, shapes[j]->c2Body, NULL, shapes[j]->type, &m);
	//		manafolds.push_back(m);
	//	}
	//}
	}

	//for (size_t i = 0; i < results.size(); i++)
		//results[i].wait();

	//using entt event dispatcher
	for (size_t i = 0; i < collisionPairs.size(); i++)
		dispatcher->trigger(collisionPairs[i]);

	//std::cout << "Mans:" << manafolds.size() << " CoPair:" << collisionPairs.size() << " StaticCount:" << staticShapesAll.size() << std::endl;
}

void CuteC2System::Render()
{
	auto cam = registry->view<Camera2D>();
	for (auto [entity, camera] : cam.each()) {
		BeginMode2D(camera);

		for (size_t i = 0; i < staticShapesAll.size(); i++)
			staticShapesAll[i]->DebugDraw();

		for (size_t i = 0; i < dynamicShapes.size(); i++)
			dynamicShapes[i]->DebugDraw();

		for (size_t i = 0; i < manafolds.size(); i++)
			DrawMan(manafolds[i]);
		EndMode2D();
	}
}

template<typename T>
void CuteC2System::ProcessDynamics()
{
	static_assert(std::is_base_of<BaseShape, T>::value, "T must derive from Base");

	auto view = registry->view<Position, T>();
	for (auto [entity, position, shape] : view.each())
	{
		if (shape.isStatic)
			continue;

		//update points on shape
		shape.Move(position.position, position.offset, position.rotation);

		/*Vector2 pos = shape.GetPostion();
		int x = (int)(pos.x / GRID_CELL_SIZE);
		int y = (int)(pos.x / GRID_CELL_SIZE);
		dynamicShapesMap[std::pair<int, int>{x, y}].push_back(&shape);*/

		dynamicShapes.push_back(&shape);
	}
}

template<typename T>
void CuteC2System::OnComponentAdd(entt::registry& reg, entt::entity entity)
{
	static_assert(std::is_base_of<BaseShape, T>::value, "T must derive from Base");

	BaseShape* shape = reg.try_get<T>(entity);
	shape->entity = entity;
	if (shape != NULL && shape->isStatic)
	{
		Vector2 pos = shape->GetPostion();
		int x = (int)(pos.x / GRID_CELL_SIZE);
		int y = (int)(pos.x / GRID_CELL_SIZE);
		staticShapes[std::pair<int, int>{x, y}].push_back(shape);
		staticShapesAll.push_back(shape);
	}
}

template<typename T>
void CuteC2System::OnComponentRemove(entt::registry& reg, entt::entity entity)
{
	static_assert(std::is_base_of<BaseShape, T>::value, "T must derive from Base");

	BaseShape* shape = reg.try_get<T>(entity);
	if (shape != NULL && shape->isStatic)
	{
		Vector2 pos = shape->GetPostion();
		int x = (int)(pos.x / GRID_CELL_SIZE);
		int y = (int)(pos.x / GRID_CELL_SIZE);

		remove(staticShapes[std::pair<int, int>{x, y}].begin(), staticShapes[std::pair<int, int>{x, y}].end(), shape);
		remove(staticShapesAll.begin(), staticShapesAll.end(), shape);
	}
}
