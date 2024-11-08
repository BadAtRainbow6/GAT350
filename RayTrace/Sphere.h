#pragma once
#include "SceneObject.h"

#include <glm/glm.hpp>

class Sphere : public SceneObject
{
public:
	Sphere() = default;
	Sphere(const Transform& transform, float radius, std::shared_ptr<Material> material) :
		SceneObject{ transform, material },
		m_radius{ radius }
	{}
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : 
		m_center{ center }, 
		m_radius{ radius }, 
		SceneObject{ material } 
	{}

	bool Hit(const Ray& ray, RaycastHit& raycastHit, float minDistance, float maxDistance);
	static bool Raycast(const Ray& ray, const glm::vec3& center, float radius, float minDistance, float maxDistance, float& t);

private:
	glm::vec3 m_center{ 0 };
	float m_radius{ 0 };
};