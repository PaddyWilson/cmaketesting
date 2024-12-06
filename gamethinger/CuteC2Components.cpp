#include "CuteC2Components.h"

#include "Vector2Math.h"
#include "CuteMath.h"

void DrawRaycast(c2v impact, c2v end)
{
	DrawLine(impact.x * CUTE_SCALE_FACTOR, impact.y * CUTE_SCALE_FACTOR, end.x * CUTE_SCALE_FACTOR, end.y * CUTE_SCALE_FACTOR, GREEN);
	DrawCircle(impact.x * CUTE_SCALE_FACTOR, impact.y * CUTE_SCALE_FACTOR, 4, PINK);
	DrawCircle(end.x * CUTE_SCALE_FACTOR, end.y * CUTE_SCALE_FACTOR, 4, RED);
}

void DrawMan(c2Manifold& m)
{
	c2v n = m.n;
	//gl_line_color(ctx, 1.0f, 0.2f, 0.4f);
	for (int i = 0; i < m.count; ++i)
	{
		c2v p = m.contact_points[i];
		float d = m.depths[i];

		auto v1 = Vector2{ p.x, p.y };
		auto v2 = Vector2{ p.x + (n.x * d), p.y + (n.y * d) };

		DrawLineEx(v1 * CUTE_SCALE_FACTOR, v2 * CUTE_SCALE_FACTOR, 3, BLUE);
	}
}

BB_AABB::BB_AABB(float x, float y, float hx, float hy, Color color) {
	type = C2_TYPE_AABB;

	halfSize = Vector2(hx, hy);

	aabb.min.x = x - halfSize.x;
	aabb.min.y = y - halfSize.y;
	aabb.max.x = x + halfSize.x;
	aabb.max.y = y + halfSize.y;

	c2Body = &aabb;

	this->debugColor = color;
}

BB_AABB::BB_AABB(Vector2 position, Vector2 halftSize, bool isStatic, Color color)
{
	type = C2_TYPE_AABB;
	this->isStatic = isStatic;

	this->halfSize = halftSize;
	position = position;

	aabb.min.x = position.x - halfSize.x;
	aabb.min.y = position.y - halfSize.y;
	aabb.max.x = position.x + halfSize.x;
	aabb.max.y = position.y + halfSize.y;

	c2Body = &aabb;

	this->debugColor = color;
}

void BB_AABB::Move(Vector2 position, Vector2 offset, float rotation)
{
	aabb.min.x = (position.x) - halfSize.x;
	aabb.min.y = (position.y) - halfSize.y;
	aabb.max.x = (position.x) + halfSize.x;
	aabb.max.y = (position.y) + halfSize.y;
	c2Body = &aabb;
}

Vector2 BB_AABB::GetPostion()
{
	return Vector2(aabb.min.x + halfSize.x, aabb.min.y + halfSize.y);
}

void BB_AABB::DebugDraw() {
	DrawLine(aabb.min.x * CUTE_SCALE_FACTOR, aabb.min.y * CUTE_SCALE_FACTOR, aabb.max.x * CUTE_SCALE_FACTOR, aabb.min.y * CUTE_SCALE_FACTOR, debugColor);
	DrawLine(aabb.max.x * CUTE_SCALE_FACTOR, aabb.min.y * CUTE_SCALE_FACTOR, aabb.max.x * CUTE_SCALE_FACTOR, aabb.max.y * CUTE_SCALE_FACTOR, debugColor);
	DrawLine(aabb.max.x * CUTE_SCALE_FACTOR, aabb.max.y * CUTE_SCALE_FACTOR, aabb.min.x * CUTE_SCALE_FACTOR, aabb.max.y * CUTE_SCALE_FACTOR, debugColor);
	DrawLine(aabb.min.x * CUTE_SCALE_FACTOR, aabb.max.y * CUTE_SCALE_FACTOR, aabb.min.x * CUTE_SCALE_FACTOR, aabb.min.y * CUTE_SCALE_FACTOR, debugColor);
}

BB_CIRCLE::BB_CIRCLE(Vector2 position, float radius, bool isStatic, Color color)
{
	type = C2_TYPE_CIRCLE;
	this->isStatic = isStatic;
	this->radius = radius;
	//this->position = position;

	circle.p.x = position.x;
	circle.p.y = position.y;
	circle.r = radius;

	c2Body = &circle;

	this->debugColor = color;
}

void BB_CIRCLE::Move(Vector2 position, Vector2 offset, float rotation)
{
	circle.p.x = (position.x);
	circle.p.y = (position.y);
	c2Body = &circle;//this should not have to happen but it does
}

Vector2 BB_CIRCLE::GetPostion()
{
	return Vector2(circle.p.x, circle.p.y);
}

void BB_CIRCLE::DebugDraw() {
	DrawCircleLines(circle.p.x * CUTE_SCALE_FACTOR, circle.p.y * CUTE_SCALE_FACTOR, circle.r * CUTE_SCALE_FACTOR, debugColor);
}

BB_POLY::BB_POLY()
{
	type = C2_TYPE_POLY;

	Vector2 halfSize{ 0.5f,0.5f };
	points.push_back(Vector2{ (-halfSize.x), (-halfSize.y) });
	points.push_back(Vector2{ (-halfSize.x), (-halfSize.y) });
	points.push_back(Vector2{ (halfSize.x), (-halfSize.y) });
	points.push_back(Vector2{ (halfSize.x), (-halfSize.y) });
	points.push_back(Vector2{ (halfSize.x), (halfSize.y) });
	points.push_back(Vector2{ (halfSize.x), (halfSize.y) });
	points.push_back(Vector2{ (-halfSize.x), (halfSize.y) });
	points.push_back(Vector2{ (-halfSize.x), (halfSize.y) });

	for (size_t i = 0; i < points.size(); i++) {
		poly.verts[i].x = position.x + points[i].x;
		poly.verts[i].y = position.y + points[i].y;
	}

	poly.count = points.size();
	c2MakePoly(&poly);
	c2Body = &poly;
	this->debugColor = RED;
}

BB_POLY::BB_POLY(Vector2 position, std::vector<Vector2> points, bool isStatic, Color color)
{
	type = C2_TYPE_POLY;
	this->isStatic = isStatic;
	this->position = position;

	for (size_t i = 0; i < points.size(); i++) {
		this->points.push_back(points[i]);
		//points[i] = this->position + points[i];
	}

	for (size_t i = 0; i < points.size(); i++) {
		poly.verts[i].x = position.x + points[i].x;
		poly.verts[i].y = position.y + points[i].y;
	}

	poly.count = points.size();
	c2MakePoly(&poly);
	c2Body = &poly;
	this->debugColor = color;
}

void BB_POLY::Move(Vector2 position, Vector2 offset, float rotation)
{
	this->position = position;

	c2r rotationMatrix = c2Rot(rotation * (3.14195 / 180));
	for (size_t i = 0; i < points.size(); i++) {
		//rotate points around 0,0
		poly.verts[i] = c2Mulrv(rotationMatrix, c2v{ points[i].x, points[i].y });
		//add position to points
		poly.verts[i].x += position.x;
		poly.verts[i].y += position.y;
	}

	poly.count = points.size();
	c2MakePoly(&poly);
	c2Body = &poly;
}

Vector2 BB_POLY::GetPostion()
{
	return position;
}

void BB_POLY::DebugDraw() {
	if (poly.count == 0)
	{
		printf("no verts\n");
		return;
	}

	for (size_t i = 0; i < poly.count - 1; i++)
	{
		DrawLine(
			poly.verts[i].x * CUTE_SCALE_FACTOR,
			poly.verts[i].y * CUTE_SCALE_FACTOR,
			poly.verts[i + 1].x * CUTE_SCALE_FACTOR,
			poly.verts[i + 1].y * CUTE_SCALE_FACTOR,
			debugColor);
	}
	DrawLine(
		poly.verts[poly.count - 1].x * CUTE_SCALE_FACTOR,
		poly.verts[poly.count - 1].y * CUTE_SCALE_FACTOR,
		poly.verts[0].x * CUTE_SCALE_FACTOR,
		poly.verts[0].y * CUTE_SCALE_FACTOR,
		debugColor);
}

BB_CAP::BB_CAP(Vector2 position, Vector2 p1, Vector2 p2, float radius, bool isStatic, Color color)
{
	type = C2_TYPE_CAPSULE;
	this->isStatic = isStatic;

	this->p1 = p1;
	this->p2 = p2;

	position = position;
	this->position = position;
	this->radius = radius;

	cap.a.x = position.x + p1.x;
	cap.a.y = position.y + p1.y;

	cap.b.x = position.x + p2.x;
	cap.b.y = position.y + p2.y;

	cap.r = radius;

	c2Body = &cap;

	this->debugColor = color;
}

void BB_CAP::Move(Vector2 position, Vector2 offset, float rotation)
{
	this->position = position;

	cap.a.x = position.x + p1.x;
	cap.a.y = position.y + p1.y;

	cap.b.x = position.x + p2.x;
	cap.b.y = position.y + p2.y;

	cap.r = radius;

	c2v p{ this->position.x, this->position.y };
	c2r r = c2Rot(rotation * (3.14195 / 180));
	//rotate p1
	c2v n{ cap.a.x - p.x, cap.a.y - p.y };
	n = c2Mulrv(r, n);
	n.x += p.x;
	n.y += p.y;
	cap.a = n;

	//rotate p2
	n = { cap.b.x - p.x, cap.b.y - p.y };
	n = c2Mulrv(r, n);
	n.x += p.x;
	n.y += p.y;
	cap.b = n;

	c2Body = &cap;
}

Vector2 BB_CAP::GetPostion()
{
	return position;
}

void BB_CAP::DrawHalfCircle(c2v a, c2v b)
{
	c2v u = c2Sub(b, a);
	float r = c2Len(u);
	u = c2Skew(u);
	c2v v = c2CCW90(u);
	c2v s = c2Add(v, a);
	c2m m;
	m.x = c2Norm(u);
	m.y = c2Norm(v);

	int kSegs = 20;
	float theta = 0;
	float inc = 3.14159265f / (float)kSegs;
	c2v p0;
	c2SinCos(theta, &p0.y, &p0.x);
	p0 = c2Mulvs(p0, r);
	p0 = c2Add(c2Mulmv(m, p0), a);
	for (int i = 0; i < kSegs; ++i)
	{
		theta += inc;
		c2v p1;
		c2SinCos(theta, &p1.y, &p1.x);
		p1 = c2Mulvs(p1, r);
		p1 = c2Add(c2Mulmv(m, p1), a);
		DrawLine(p0.x * CUTE_SCALE_FACTOR, p0.y * CUTE_SCALE_FACTOR, p1.x * CUTE_SCALE_FACTOR, p1.y * CUTE_SCALE_FACTOR, debugColor);
		p0 = p1;
	}
}

void BB_CAP::DebugDraw() {
	//DrawCircle
	c2v a = cap.a;
	c2v b = cap.b;
	float r = cap.r;

	c2v n = c2Norm(c2Sub(b, a));
	DrawHalfCircle(a, c2Add(a, c2Mulvs(n, -r)));
	DrawHalfCircle(b, c2Add(b, c2Mulvs(n, r)));
	c2v p0 = c2Add(a, c2Mulvs(c2Skew(n), r));
	c2v p1 = c2Add(b, c2Mulvs(c2CCW90(n), -r));
	DrawLine(p0.x * CUTE_SCALE_FACTOR, p0.y * CUTE_SCALE_FACTOR, p1.x * CUTE_SCALE_FACTOR, p1.y * CUTE_SCALE_FACTOR, debugColor);
	p0 = c2Add(a, c2Mulvs(c2Skew(n), -r));
	p1 = c2Add(b, c2Mulvs(c2CCW90(n), r));
	DrawLine(p0.x * CUTE_SCALE_FACTOR, p0.y * CUTE_SCALE_FACTOR, p1.x * CUTE_SCALE_FACTOR, p1.y * CUTE_SCALE_FACTOR, debugColor);
}

BB_RECTANGLE::BB_RECTANGLE()
{
	type = C2_TYPE_POLY;
	this->halfSize = Vector2{ 0.5f, 0.5f };
	this->debugColor = BLUE;

	rectangle.verts[0] = c2v{ (-halfSize.x), (-halfSize.y) };
	rectangle.verts[1] = c2v{ (halfSize.x), (-halfSize.y) };
	rectangle.verts[2] = c2v{ (halfSize.x), (halfSize.y) };
	rectangle.verts[3] = c2v{ (-halfSize.x), (halfSize.y) };
	rectangle.count = 4;

	c2MakePoly(&rectangle);
	c2Body = &rectangle;
}

BB_RECTANGLE::BB_RECTANGLE(Vector2 position, Vector2 halfSize, bool isStatic, Color color)
{
	type = C2_TYPE_POLY;
	this->isStatic = isStatic;
	this->halfSize = halfSize;
	this->position = position;
	this->debugColor = color;

	rectangle.verts[0] = c2v{ position.x + (-halfSize.x) ,	position.y + (-halfSize.y) };
	rectangle.verts[1] = c2v{ position.x + (halfSize.x),	position.y + (-halfSize.y) };
	rectangle.verts[2] = c2v{ position.x + (halfSize.x),	position.y + (halfSize.y) };
	rectangle.verts[3] = c2v{ position.x + (-halfSize.x),	position.y + (halfSize.y) };
	rectangle.count = 4;

	c2MakePoly(&rectangle);
	c2Body = &rectangle;
}

void BB_RECTANGLE::Move(Vector2 position, Vector2 offset, float rotation)
{
	this->position = position;

	rectangle.verts[0] = c2v{ (-halfSize.x),(-halfSize.y) };
	rectangle.verts[1] = c2v{ (halfSize.x),(-halfSize.y) };
	rectangle.verts[2] = c2v{ (halfSize.x),(halfSize.y) };
	rectangle.verts[3] = c2v{ (-halfSize.x),(halfSize.y) };
	rectangle.count = 4;

	c2r rotationMatrix = c2Rot(rotation * (3.14195 / 180));
	for (size_t i = 0; i < 4; i++) {
		//rotate points around 0,0
		rectangle.verts[i] = c2Mulrv(rotationMatrix, rectangle.verts[i]);
		//add position to points
		rectangle.verts[i].x += position.x;
		rectangle.verts[i].y += position.y;
	}

	c2Body = &rectangle;
}

Vector2 BB_RECTANGLE::GetPostion()
{
	return position;
}

void BB_RECTANGLE::DebugDraw()
{
	for (size_t i = 0; i < rectangle.count - 1; i++)
	{
		DrawLine(rectangle.verts[i].x * CUTE_SCALE_FACTOR,
			rectangle.verts[i].y * CUTE_SCALE_FACTOR,
			rectangle.verts[i + 1].x * CUTE_SCALE_FACTOR,
			rectangle.verts[i + 1].y * CUTE_SCALE_FACTOR,
			debugColor);
	}
	DrawLine(rectangle.verts[rectangle.count - 1].x * CUTE_SCALE_FACTOR,
		rectangle.verts[rectangle.count - 1].y * CUTE_SCALE_FACTOR,
		rectangle.verts[0].x * CUTE_SCALE_FACTOR,
		rectangle.verts[0].y * CUTE_SCALE_FACTOR,
		debugColor);
}
