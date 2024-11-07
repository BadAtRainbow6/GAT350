#include "Material.h"
#include "Random.h"

bool Lambertian::Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scatter)
{
	scatter.origin = raycastHit.point;
	scatter.direction = raycastHit.normal + randomOnUnitSphere();
	attenuation = m_albedo;

	return true;
}

bool Metal::Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scattered)
{
	glm::vec3 reflected = Reflect(ray.direction, raycastHit.normal);

	// set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
	// a mirror has a fuzz value of 0 and a diffused metal surface a higher value
	scattered = Ray{ raycastHit.point, reflected + (randomOnUnitSphere() * m_fuzz) };
	attenuation = m_albedo;

	// check that reflected ray is going away from surface normal (dot product > 0)
	return Dot(scattered.direction, raycastHit.normal) > 0;
}

bool Dielectric::Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scattered)
{
	glm::vec3 outNormal;
	float ni_over_nt;
	float cosine;

	if (Dot(ray.direction, raycastHit.normal) < 0)
	{
		outNormal = raycastHit.normal;
		ni_over_nt = 1.0f / m_refractiveIndex;
		cosine = -Dot(ray.direction, raycastHit.normal) / ray.direction.length();
	}
	else
	{
		outNormal = -raycastHit.normal;
		ni_over_nt = m_refractiveIndex;
		cosine = m_refractiveIndex * Dot(ray.direction, raycastHit.normal) / glm::length(ray.direction);
	}

	glm::vec3 refracted;
	float reflectProbability = 1.0f;

	if (Refract(ray.direction, outNormal, ni_over_nt, refracted))
	{
		reflectProbability = Schlick(cosine, m_refractiveIndex);
	}
	glm::vec3 reflected = Reflect(ray.direction, raycastHit.normal);

	scattered = (randomf() < reflectProbability) ? Ray{ raycastHit.point, reflected } : Ray{ raycastHit.point, refracted };

	attenuation = m_albedo;

	return true;
}
