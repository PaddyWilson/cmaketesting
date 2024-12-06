#include "CuteMath.h"

//add
c2v operator+(const c2v v1, const c2v v2)
{
	return c2Add(v1, v2);
}

c2v operator+(const c2v vec, float val)
{
	return c2v(vec.x + val, vec.y + val);
}

c2v& operator+=(c2v& v1, c2v v2) {
	v1 = v1 + v2;
	return v1;
}

c2v& operator+=(c2v& v1, float v2) {
	v1 = v1 + v2;
	return v1;
}


//sub
c2v operator-(const c2v v1, const c2v v2)
{
	return c2Sub(v1, v2);
}

c2v operator-(const c2v vec, float val)
{
	return c2v(vec.x - val, vec.y - val);
}

c2v& operator-=(c2v& v1, c2v v2)
{
	v1 = v1 - v2;
	return v1;
}

c2v& operator-=(c2v& v1, float v2)
{
	v1 = v1 - v2;
	return v1;
}


//mult
c2v operator*(const c2v v1, const c2v v2)
{
	return c2Mulvv(v1, v2);
}

c2v operator*(const c2v vec, float val)
{
	return c2v(vec.x * val, vec.y * val);
}

c2v& operator*=(c2v& v1, c2v v2)
{
	v1 = v1 * v2;
	return v1;
}

c2v& operator*=(c2v& v1, float v2)
{
	v1 = v1 * v2;
	return v1;
}

//div
c2v operator/(const c2v v1, const c2v v2)
{
	return c2v(v1.x * v2.x, v1.y * v2.y);
}

c2v operator/(const c2v vec, float val)
{
	return c2Div(vec, val);
}

c2v& operator/=(c2v& v1, c2v v2)
{
	v1 = v1 / v2;
	return v1;
}

c2v& operator/=(c2v& v1, float v2)
{
	v1 = v1 / v2;
	return v1;
}

inline c2v floor(c2v vec)
{
	return c2v(floor(vec.x), floor(vec.y));
}

inline c2v CToV(const Vector2 vec)
{
	return c2v(vec.x, vec.y);
}

inline Vector2 VToC(const c2v vec)
{
	return Vector2(vec.x, vec.y);
}

