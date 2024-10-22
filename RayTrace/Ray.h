#pragma once
#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 m_origin;
	glm::vec3 m_direction;

	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin{ origin }, m_direction{ direction } {}

	glm::vec3 At(float t) const { return m_origin + m_direction * t; }

	glm::vec3 operator * (float t) { return At(t); }
};