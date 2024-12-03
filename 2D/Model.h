#pragma once
#include "Color.h"
#include "Camera.h"
#include "VertexShader.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>



class Model
{
public:
		Model() = default;
		Model(const vertexbuffer_t& verticies, const color4_t& color) : m_vb{ verticies }, m_color{ color } {}

		//void SetColor(const color4_t& newColor);
		void Draw();
		bool Load(const std::string& filename);

private:
	vertexbuffer_t m_vb;
	color4_t m_color { 1 };

};