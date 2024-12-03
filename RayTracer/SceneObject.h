#pragma once
#include "Material.h"
#include "Transform.h"
#include <memory>
#include "Ray.h"
class SceneObject 
{
public:
	//SceneObject() = default;
	SceneObject(std::shared_ptr<Material> material) : m_material{ material } {}
	SceneObject(Transform transform, std::shared_ptr<Material> material) : m_transform{ transform }, m_material{ material } {}
	
	virtual bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float MaxDistance) = 0;
	virtual void Update() {}
	std::weak_ptr<Material> GetMaterial() { return m_material; }
	Transform& GetTransform() { return m_transform; }
protected:
	std::shared_ptr<Material> m_material;
	Transform m_transform;
};

class Triangle : public SceneObject
{
public:
	Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, std::shared_ptr<Material> material) : SceneObject{ material }, 
		m_v1{ v1 },
		m_v2{ v2 },
		m_v3{ v3 }
	{}

	Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, std::shared_ptr<Material> material, Transform& transform) : 
		SceneObject{ transform, material },
		m_v1{ v1 },
		m_v2{ v2 },
		m_v3{ v3 }
	{}

	//Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, std::shared_ptr<Material> material, Transform& transform) :

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, float minDistance, float MaxDistance, float& t);
	
	void Update() override;
private:
	glm::vec3 m_v1{ 0, 0, 0 };
	glm::vec3 m_v2{ 0, 0, 0 };
	glm::vec3 m_v3{ 0, 0, 0 };

	glm::vec3 m_local_v1{ 0, 0, 0 };
	glm::vec3 m_local_v2{ 0, 0, 0 };
	glm::vec3 m_local_v3{ 0, 0, 0 };
	// Inherited via SceneObject
	//bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float MaxDistance) override;
};