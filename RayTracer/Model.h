#pragma once
#include "Color.h"
#include "Camera.h"
#include "SceneObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

using vertex_t = glm::vec3;
using vertexbuffer_t = std::vector<vertex_t>;

class Model : public SceneObject
{
public:
	Model(std::shared_ptr<Material> material) : SceneObject{ material } {}
	Model(const vertexbuffer_t& vertices, std::shared_ptr<Material> material) : SceneObject{ material }, m_vb{ vertices } {}
	
	Model(const Transform& transform, const vertexbuffer_t& verticies, std::shared_ptr<Material> material) :
		SceneObject{ transform, material },
		m_vb{ verticies }
	{}
	//void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
	bool Load(const std::string& filename);
	void Update();
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	vertexbuffer_t m_vb;
	vertexbuffer_t m_local_vertices;
};