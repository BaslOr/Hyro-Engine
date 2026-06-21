#pragma once
#include <glm/glm.hpp>

namespace Hyro {

	//Until now the Vertex Layout is fixed
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec4 Color;

		Vertex()
			: Position(0.f, 0.f, 0.f), UV(0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f)
		{

		}

		Vertex(const glm::vec3& position, const glm::vec2& uv, const glm::vec4& color)
			: Position(position), UV(uv), Color(color)
		{
		}

		Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a)
			: Position({ x, y, z }), UV({u, v}), Color({r, g, b, a})
		{
		}
	};

}
