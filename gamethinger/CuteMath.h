#pragma once

#include "cute_c2.h"
#include "raymath.h"

//add
inline c2v operator+(const c2v v1, const c2v v2);
inline c2v operator+(c2v vec, float val);
inline c2v& operator+=(c2v& v1, c2v v2);
inline c2v& operator+=(c2v& v1, float v2);

//sub
inline c2v operator-(const c2v v1, const c2v v2);
inline c2v operator-(const c2v vec, float val);
inline c2v& operator-=(c2v& v1, c2v v2);
inline c2v& operator-=(c2v& v1, float v2);

//mult
inline c2v operator*(const c2v v1, const c2v v2);
inline c2v operator*(const c2v vec, float val);
inline c2v& operator*=(c2v& v1, c2v v2);
inline c2v& operator*=(c2v& v1, float v2);

//div
inline c2v operator/(const c2v v1, const c2v v2);
inline c2v operator/(const c2v vec, float val);
inline c2v& operator/=(c2v& v1, c2v v2);
inline c2v& operator/=(c2v& v1, float v2);

inline c2v floor(c2v vec);

//convert functions
//Vector2 to c2v
inline c2v CToV(const Vector2 v2);
//ct2 to Vector2
inline Vector2 VToC(const c2v v1);