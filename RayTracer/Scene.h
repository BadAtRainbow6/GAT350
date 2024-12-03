#pragma once
#include <vector>
#include <memory>
#include "Sphere.h"
#include "Framebuffer.h"

class Scene 
{
public:
	Scene() = default;
	void Update();
	void AddObject(std::unique_ptr<class SceneObject> object) { m_objects.push_back(std::move(object)); }

	void Render(Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	
	void SetSky(const color3_t& skyBottom, const color3_t& skyTop)
	{
		m_skyBottom = skyBottom;
		m_skyTop = skyTop;
	}

	friend class Tracer;

private:
	std::vector<std::unique_ptr<class SceneObject>> m_objects;
	color3_t m_skyBottom{ 1 };
	color3_t m_skyTop{ 0.5f, 0.7f, 1.0f };
};
