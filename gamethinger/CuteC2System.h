#pragma once
#include "BaseSystem.h"

#include "Components.h"
#include "CuteC2Components.h"

#include <cute_c2.h>
#include <vector>
#include <unordered_map>

// #include <concurrent_vector.h>
// #include <concurrent_unordered_map.h>

// if you have big collision boxes increase one or both
#define GRID_CELL_SIZE 10
#define GRID_SEARCH_RADIUS 1

// used to has pairs for unordered_map
struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const
	{
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ h2;
	}
};

class CuteC2System : public BaseSystem
{
private:
	std::unordered_map<std::pair<int, int>, std::vector<BaseShape *>, pair_hash> dynamicShapesMap;
	std::vector<BaseShape *> dynamicShapes;
	std::vector<c2Manifold> manafolds;
	std::vector<CollisionPair> collisionPairs;
	std::vector<BaseShape *> staticShapesAll;
	std::unordered_map<std::pair<int, int>, std::vector<BaseShape *>, pair_hash> staticShapes;

	/*std::unordered_map<std::pair<int, int>, std::vector<BaseShape*>, pair_hash> dynamicShapesMap;
	std::vector<BaseShape*> dynamicShapes;
	std::vector<c2Manifold> manafolds;
	std::vector<CollisionPair> collisionPairs;

	std::vector<BaseShape*> staticShapesAll;
	std::unordered_map<std::pair<int, int>, std::vector<BaseShape*>, pair_hash> staticShapes;*/
public:
	CuteC2System();

	void Init();
	// void End() {}
	void Reset();

	// void PreUpdate(float dt) {}
	void Update(float deltaTime);
	// void PostUpdate(float dt) {}

	void Render();
	// void RenderUI() {}

private:
	template <typename T>
	void ProcessDynamics();
	template <typename T>
	void OnComponentAdd(entt::registry &reg, entt::entity entity);
	template <typename T>
	void OnComponentRemove(entt::registry &reg, entt::entity entity);
};