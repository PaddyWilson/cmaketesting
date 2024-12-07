#pragma once

#include <raylib.h>
#include <vector>
#include <entt/entt.hpp>
//phys
#ifndef CUTE_C2_IMPLEMENTATION
#define CUTE_C2_IMPLEMENTATION
#endif
#include <cute_c2.h>

//use to scale cute_c2 positions to screen position;
// cute it does not work great with big numbers
// 1 cute unit = x screen unit
#define CUTE_SCALE_FACTOR 10.f

void DrawRaycast(c2v impact, c2v end);
void DrawMan(c2Manifold& m);

class BaseShape {
public:
	C2_TYPE type;
	void* c2Body;
	Color debugColor = RED;
	bool isStatic = false;
	entt::entity entity = entt::null;

	virtual void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0) {};
	virtual Vector2 GetPostion() = 0;
	virtual void DebugDraw() = 0;
};

struct CollisionPair
{
	entt::entity entity1;
	entt::entity entity2;
	//BaseShape* shape1;
	//BaseShape* shape2;
	//c2Manifold manifold;
};

class BB_AABB :public BaseShape {
public:
	Vector2 halfSize = { 0 };

	c2AABB aabb;

	BB_AABB() {
		c2Body = &aabb;
	};
	BB_AABB(float x, float y, float hx, float hy, Color color = RED);
	BB_AABB(Vector2 position, Vector2 halftSize, bool isStatic = true, Color color = RED);
	void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0);
	Vector2 GetPostion();
	void DebugDraw();
};

class BB_RECTANGLE :public BaseShape {
public:
	Vector2 halfSize{ 0.5f };
	Vector2 position;

	c2Poly rectangle;

	BB_RECTANGLE();
	BB_RECTANGLE(Vector2 position, Vector2 halftSize, bool isStatic = true, Color color = DARKPURPLE);
	void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0);
	Vector2 GetPostion();
	void DebugDraw();
};

class BB_CIRCLE :public BaseShape {
public:
	float radius = 0.5f;

	c2Circle circle;

	BB_CIRCLE() {
		circle.r = 1;
		c2Body = &circle;
	};
	BB_CIRCLE(Vector2 position, float radius, bool isStatic = true, Color color = RED);
	void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0);
	Vector2 GetPostion();
	void DebugDraw();
};

class BB_POLY :public BaseShape {
public:
	//points are relative from the position
	Vector2 position{ 0.f,0.f };
	std::vector<Vector2> points;

	c2Poly poly;

	BB_POLY();
	BB_POLY(Vector2 position, std::vector<Vector2> points, bool isStatic = true, Color color = BLUE);

	void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0);
	Vector2 GetPostion();
	void DebugDraw();
};

class BB_CAP :public BaseShape {
public:
	Vector2 position{ 0.f,0.f };
	Vector2 p1{ -1.f,0.f }, p2{ 1.f,0.f };
	float radius = 0.5;

	c2Capsule cap;

	BB_CAP() {};
	BB_CAP(Vector2 position, Vector2 p1, Vector2 p2, float radius, bool isStatic = true, Color color = PURPLE);

	void Move(Vector2 position, Vector2 offset = Vector2{ 0.f,0.f }, float rotation = 0);
	Vector2 GetPostion();
	void DrawHalfCircle(c2v a, c2v b);
	void DebugDraw();
};