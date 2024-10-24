#pragma once
#include <glm/glm.hpp>
#include <memory>

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction) : origin{ origin }, direction{ direction } {}

	glm::vec3 At(float t) const { return origin + direction * t; }

	glm::vec3 operator * (float t) { return At(t); }
};

struct RaycastHit
{
	float distance{ 0 };

	glm::vec3 point{ 0 };
	glm::vec3 normal{ 0 };

	std::weak_ptr<class Material> material;
};