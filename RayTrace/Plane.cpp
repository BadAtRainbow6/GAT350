#include "Plane.h"
#include "MathUtils.h"

bool Plane::Hit(const Ray& ray, RaycastHit& raycastHit, float minDistance, float maxDistance)
{
    float t;
    if (!Raycast(ray, m_transform.position, m_transform.GetUp(), minDistance, maxDistance, t)) return false;

    raycastHit.distance = t;
    raycastHit.normal = glm::normalize(m_transform.GetUp());
    raycastHit.point = ray.At(t);
    raycastHit.material = GetMaterial();

    return true;
}

bool Plane::Raycast(const Ray& ray, const glm::vec3& point, const glm::vec3& normal, float minDistance, float maxDistance, float& t)
{
    // check dot product of ray direction and plane normal, if result is 0 then ray direction if parallel to plane so it never hits plane
    // the dot product is 0 if the two vectors are perpendicular (90 degrees)
    float denominator = Dot(ray.direction, normal);
    if (Approximately(denominator, 0))
    {
        return false; // ray direction and plane parallel, no intersection
    }

    // get t (distance) along ray direction to hit point
    t = (Dot(normal, (point - ray.origin)) / denominator); //<the dot product of(center - ray origin) and the plane normal> / denominator
    if (t < 0)
    {
        return false; // plane behind ray's origin, no intersection
    }

    if (t <= minDistance || t >= maxDistance) return false;

    return true;
}
