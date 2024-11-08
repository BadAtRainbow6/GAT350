#pragma once
#include "SceneObject.h"

class Plane : public SceneObject
{
public:
	Plane() = default;
	Plane(const Transform& transform, const std::shared_ptr<Material> material) :
		SceneObject{ transform, material }
	{}
	Plane(const glm::vec3& center, const glm::vec3& normal, std::shared_ptr<Material> material) :
		SceneObject{ material },
		m_center{ center },
		m_normal{ normal }
	{}

	bool Hit(const Ray& ray, RaycastHit& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const Ray& ray, const glm::vec3& point, const glm::vec3& normal, float minDistance, float maxDistance, float& t);

private:
	glm::vec3 m_center;
	glm::vec3 m_normal;
};