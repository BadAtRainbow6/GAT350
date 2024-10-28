#include "Material.h"
#include "Random.h"

bool Lambertian::Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scatter)
{
	scatter.origin = raycastHit.point;
	scatter.direction = raycastHit.normal + randomOnUnitSphere();
	attenuation = m_albedo;

	return true;
}