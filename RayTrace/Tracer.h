#pragma once
#include "Framebuffer.h"
#include "Ray.h"
#include "Camera.h"

class Tracer
{
public:
	static color3_t Trace(class Scene& scene, const Ray& ray, float minDistance, float maxDistance, int depth);
};