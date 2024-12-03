#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

class Camera 
{
	public:
		Camera(int width, int height) : screen_width{ width }, screen_height{ height } {}
		
		void SetProjection(float fov, float aspect, float near, float far);
		void SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3{0, 1, 0});

		glm::vec3 ModelToView(const glm::vec3& pos)  const;
		glm::vec4 ViewToProjection(const glm::vec3& pos) const;

		glm::ivec2 ViewToScreen(const glm::vec3& pos) const;

		const glm::mat4x4& GetView() { return m_view; }
		const glm::mat4x4& GetProjection() { return m_projection; }

	private:
		glm::mat4x4 m_view { 1 };
		glm::mat4x4 m_projection { 1 };

		int screen_width{ 0 };
		int screen_height{ 0 };
};