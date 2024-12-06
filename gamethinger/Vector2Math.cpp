// #include "Vector2Math.h"

// //add
// Vector2 operator+(const Vector2 v1, const Vector2 v2)
// {
// 	return Vector2Add(v1, v2);
// }

// Vector2 operator+(const Vector2 vec, float val)
// {
// 	return Vector2AddValue(vec, val);
// }

// Vector2& operator+=(Vector2& v1, Vector2 v2) {
// 	v1 = v1 + v2;
// 	return v1;
// }

// Vector2& operator+=(Vector2& v1, float v2) {
// 	v1 = v1 + v2;
// 	return v1;
// }


// //sub
// Vector2 operator-(const Vector2 v1, const Vector2 v2)
// {
// 	return Vector2Subtract(v1, v2);
// }

// Vector2 operator-(const Vector2 vec, float val)
// {
// 	return Vector2SubtractValue(vec, val);
// }

// Vector2& operator-=(Vector2& v1, Vector2 v2)
// {
// 	v1 = v1 - v2;
// 	return v1;
// }

// Vector2& operator-=(Vector2& v1, float v2)
// {
// 	v1 = v1 - v2;
// 	return v1;
// }


// //mult
// Vector2 operator*(const Vector2 v1, const Vector2 v2)
// {
// 	return Vector2Multiply(v1, v2);
// }

// Vector2 operator*(const Vector2 vec, float val)
// {
// 	return Vector2Scale(vec, val);
// }

// Vector2& operator*=(Vector2& v1, Vector2 v2)
// {
// 	v1 = v1 * v2;
// 	return v1;
// }

// Vector2& operator*=(Vector2& v1, float v2)
// {
// 	v1 = v1 * v2;
// 	return v1;
// }

// //div
// Vector2 operator/(const Vector2 v1, const Vector2 v2)
// {
// 	return Vector2Divide(v1, v2);
// }

// Vector2 operator/(const Vector2 vec, float val)
// {
// 	Vector2 result = { vec.x / val, vec.y / val };
// 	return result;
// }

// Vector2& operator/=(Vector2& v1, Vector2 v2)
// {
// 	v1 = v1 / v2;
// 	return v1;
// }

// Vector2& operator/=(Vector2& v1, float v2)
// {
// 	v1 = v1 / v2;
// 	return v1;
// }

// Vector2 floor(Vector2 vec)
// {
// 	return Vector2(floor(vec.x), floor(vec.y));
// }

// //other
// Vector2 rotateAroundOriginVec2(Vector2 point, Vector2 origin, float angle)
// {
// 	float angle_rad = angle / (float)(180 / 3.1459);

// 	float new_x = point.x * (float)cos(angle_rad) - point.y * (float)sin(angle_rad);
// 	float new_y = point.x * (float)sin(angle_rad) + point.y * (float)cos(angle_rad);

// 	float x = new_x;
// 	float y = new_y;

// 	Vector2 p;
// 	p.x = new_x;
// 	p.y = new_y;

// 	return point;
// }

// //RectangleBoundingBox rotateAroundOriginRec(RectangleBoundingBox rec, Vector2 origin, float angle)
// //{
// //	Vector2 topLeft = { rec.x, rec.y };
// //	Vector2 topRight{ rec.x + rec.width, rec.y };
// //	Vector2 bottomLeft{ rec.x, rec.y + rec.height };
// //	Vector2 bottomRight{ rec.x + rec.width, rec.y + rec.height };
// //
// //	Vector2 topLeftNEW{ rotateAroundOriginVec2(bottomLeft, origin, angle) };
// //	Vector2 topRightNEW{ rotateAroundOriginVec2(topLeft, origin, angle) };
// //	Vector2 bottomLeftNEW{ rotateAroundOriginVec2(bottomRight, origin, angle) };
// //	Vector2 bottomRightNEW{ rotateAroundOriginVec2(topRight, origin, angle) };
// //
// //
// //	return RectangleBoundingBox{ topLeftNEW.x, topLeftNEW.y, rec.height, rec.width };
// //}