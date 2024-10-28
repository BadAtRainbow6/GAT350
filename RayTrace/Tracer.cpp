#include "Tracer.h"
#include "MathUtils.h"
#include "Scene.h"

color3_t Tracer::Trace(Scene& scene, const Ray ray, float minDistance, float maxDistance)
{
    RaycastHit raycastHit;
    float closestDistance = maxDistance;
    bool isHit = false;

    for (auto& object : scene.m_objects)
    {
        if (object->Hit(ray, raycastHit, minDistance, closestDistance))
        {
            isHit = true;
            closestDistance = raycastHit.distance;
        }
    }

    if (isHit)
    {
        color3_t attenuation;
        Ray scatter;
        if (raycastHit.material.lock()->Scatter(ray, raycastHit, attenuation, scatter))
        {
            return attenuation * Trace(scene, scatter, minDistance, maxDistance);
        }
    }

    // sky
    glm::vec3 direction = glm::normalize(ray.direction);
    float t = (direction.y + 1) * 0.5f;
    color3_t color = Lerp(color3_t{ 1, 1, 1 }, color3_t{ 0.5f, 0.7f, 1.0f }, t);

    return color;
}