#pragma once
#include <glm/glm.hpp>

namespace Hyro {

	
	enum class VertexAttributeType {
		NONE,
		FLOAT, FLOAT2, FLOAT3, FLOAT4
	};

	class VertexLayout {
	public:
		void Push(VertexAttributeType type);

		inline const std::vector<VertexAttributeType>& GetVertexAttributes() const { return m_VertexAttributes; }
		inline uint32_t GetStride() const { return m_Stride; }

		uint32_t GetVertexAttributeSize(VertexAttributeType type) const;

	private:
		std::vector<VertexAttributeType> m_VertexAttributes;
		uint32_t m_Stride = 0;
	};


	inline void VertexLayout::Push(VertexAttributeType type)
	{
		uint32_t size = GetVertexAttributeSize(type);
		m_Stride += size;
		m_VertexAttributes.push_back(type);
	}

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
