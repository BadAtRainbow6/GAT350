#pragma once
#include "SceneObject.h"

#include <glm/glm.hpp>

class Sphere : public SceneObject
{
public:
	Sphere() = default;
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : 
		m_center{ center }, 
		m_radius{ radius }, 
		SceneObject{ material } 
	{}

	bool Hit(const Ray& ray);

private:
	glm::vec3 m_center{ 0 };
	float m_radius{ 0 };
};