#pragma once
#include "Color.h"
#include "Ray.h"

class Material
{
public:
	Material() = default;
	Material(const color3_t& albedo) : m_albedo{ albedo } {}

	virtual bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scatter) = 0;

	color3_t& GetColor() { return m_albedo; }

protected:
	color3_t m_albedo;
};

class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : Material{ albedo } {}

	virtual bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scatter);
};

class Metal : public Material
{
public:

	Metal(const glm::vec3& albedo, float fuzz) : Material{ albedo }, m_fuzz{ fuzz } {}
	virtual bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scattered);

protected:
	float m_fuzz = 0;
};