#pragma once
#include "glm/glm.hpp"
template<typename T>
inline T Lerp(const T& a, const T& b, float t) 
{
	return static_cast<T>(a + ((b - a) * t));
}
template<typename T>
inline T Clamp(const T& value, const T& min, const T& max) 
{
	return (value < min) ? min : (value > max) ? max : value;
}
inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
{
	float one_minus_t1 = 1 - t;
	float a1 = one_minus_t1 * one_minus_t1;
	float b1 = (2 * one_minus_t1) * t;
	float c1 = t * t;

	x = (int)(a1 * x1 + b1 * x2 + c1 * x3);
	y = (int)(a1 * y1 + b1 * y2 + c1 * y3);
}

inline void CubicPoint(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float t, int& x, int& y) 
{
	float one_minus_t1 = 1 - t;
	float a1 = one_minus_t1 * one_minus_t1 * one_minus_t1;
	float b1 = 3 * ( one_minus_t1 * one_minus_t1) * t;
	float c1 = 3 * one_minus_t1 * (t * t);
	float d1 = t * t * t;

	x = (int)((a1 * x1) + (b1 * x2) + (c1 * x3) + (d1 * x4));
	y = (int)((a1 * y1) + (b1 * y2) + (c1 * y3) + (d1 * y4));
}

inline float Cross(const glm::vec2& a, const glm::vec2& b) 
{
	return glm::cross(glm::vec3{a.x, a.y, 0}, glm::vec3{ b.x, b.y, 0 }).z;
}