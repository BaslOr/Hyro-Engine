#pragma once
#include <glm/glm.hpp>

namespace Hyro {

	//Until now the Vertex Layout is fixed
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec4 Color;
		float SpriteIndex;

		Vertex()
			: Position(0.f, 0.f, 0.f), UV(0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f), SpriteIndex(0.0f)
		{

		}

		Vertex(const glm::vec3& position, const glm::vec2& uv, const glm::vec4& color, float spriteIndex)
			: Position(position), UV(uv), Color(color), SpriteIndex(spriteIndex)
		{
		}

		Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, float spriteIndex)
			: Position({ x, y, z }), UV({u, v}), Color({r, g, b, a}), SpriteIndex(spriteIndex)
		{
		}
	};

	//class Vertex {
	//public:
	//	virtual VertexLayout GetLayout() const = 0;

	//};

	//class Vertex2D : public Vertex {
	//public:
	//	inline VertexLayout GetLayout() const override {

	//	}
	//	
	//private:
	//	glm::vec3 Position;
	//	glm::vec2 UV;
	//	glm::vec4 Color;
	//	float SpriteIndex;
	//};


	//template<typename ...T>
	//class Vertex {
	//	inline VertexLayout GetLayout() const {

	//	}

	//private:
	//	std::tuple<T> m_Types;
	//};

}
