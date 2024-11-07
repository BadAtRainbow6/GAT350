#pragma once
#include "Color.h"
#include "Ray.h"

class Material
{
public:
	Material() = default;
	Material(const color3_t& albedo) : m_albedo{ albedo } {}

	virtual bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scatter) = 0;
	virtual color3_t GetEmissive() { return color3_t{ 0, 0, 0 }; }

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

class Dielectric : public Material
{
public:

	Dielectric(const glm::vec3& albedo, float refractiveIndex) : Material{ albedo }, m_refractiveIndex{ refractiveIndex } {}
	virtual bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& attenuation, Ray& scattered);

protected:
	float m_refractiveIndex = 0;
};

class Emissive : public Material
{
public:
	Emissive(const color3_t& albedo, float intensity = 1) : Material{ albedo }, m_intensity{ intensity } {}

	bool Scatter(const Ray& ray, const RaycastHit& raycastHit, color3_t& color, Ray& scattered) { return false; }
	color3_t GetEmissive() override { return m_albedo * m_intensity; }

private:
	float m_intensity = 1;
};