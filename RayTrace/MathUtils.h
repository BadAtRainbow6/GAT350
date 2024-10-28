#pragma once
#include <glm/glm.hpp>

#define FLT_EPSILON      1.192092896e-07F 

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

inline void QuadraticPoint(int x1, int x2, int x3, int y1, int y2, int y3, float t, int& x, int& y)
{
	float one_minus_t = 1 - t;

	float a = one_minus_t * one_minus_t;
	float b = 2 * one_minus_t * t;
	float c = t * t;

	x = (int)(a * x1 + b * x2 + c * x3);
	y = (int)(a * y1 + b * y2 + c * y3);
}

inline void CubicPoint(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, float t, int& x, int& y)
{
	float one_minus_t = 1 - t;

	float a = one_minus_t * one_minus_t * one_minus_t;
	float b = 3 * one_minus_t * one_minus_t * t;
	float c = 3 * one_minus_t * t * t;
	float d = t * t * t;

	x = (int)(a * x1 + b * x2 + c * x3 + x4 * d);
	y = (int)(a * y1 + b * y2 + c * y3 + y4 * d);
}


inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3{ (v1.y * v2.z - v2.y * v1.z), (v1.z * v2.x - v2.z * v1.x), (v1.x * v2.y - v2.x * v1.y) };
}

inline bool Approximately(float value1, float value2)
{
	return (abs(value2 - value1) < FLT_EPSILON);
}