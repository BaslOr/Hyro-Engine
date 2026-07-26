#pragma once
#include <glm/glm.hpp>
#include <string>


namespace Hyro {

	enum class VertexAttributeType {
		NONE,
		FLOAT, FLOAT2, FLOAT3, FLOAT4
	};

	class VertexLayout {
	public:
		struct Element {
			uint32_t Location;
			std::string Name;
			VertexAttributeType Type;
			uint32_t Offset;
		};

		void Push(const std::string& name, uint32_t location, VertexAttributeType type);

		inline const std::vector<Element>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		uint32_t GetVertexAttributeSize(VertexAttributeType type) const;

	private:
		std::vector<Element> m_Elements;
		uint32_t m_Stride = 0;
	};


	inline void VertexLayout::Push(const std::string& name, uint32_t location, VertexAttributeType type)
	{
		uint32_t size = GetVertexAttributeSize(type);
		uint32_t offset = m_Stride;
		m_Stride += size;

		m_Elements.push_back({ location, name, type, offset });
	}

	//struct Vertex {
	//	glm::vec3 Position;
	//	glm::vec2 UV;
	//	glm::vec4 Color;
	//	float SpriteIndex;

	//	Vertex()
	//		: Position(0.f, 0.f, 0.f), UV(0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f), SpriteIndex(0.0f)
	//	{

	//	}

	//	Vertex(const glm::vec3& position, const glm::vec2& uv, const glm::vec4& color, float spriteIndex)
	//		: Position(position), UV(uv), Color(color), SpriteIndex(spriteIndex)
	//	{
	//	}

	//	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, float spriteIndex)
	//		: Position({ x, y, z }), UV({u, v}), Color({r, g, b, a}), SpriteIndex(spriteIndex)
	//	{
	//	}
	//};

	struct Vertex2D {
	public:
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec4 Color;
		float SpriteIndex;

		Vertex2D()
			: Position(0.f, 0.f, 0.f), UV(0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f), SpriteIndex(0.0f)
		{

		}

		Vertex2D(const glm::vec3& position, const glm::vec2& uv, const glm::vec4& color, float spriteIndex)
			: Position(position), UV(uv), Color(color), SpriteIndex(spriteIndex)
		{
		}

		Vertex2D(float x, float y, float z, float u, float v, float r, float g, float b, float a, float spriteIndex)
			: Position({ x, y, z }), UV({ u, v }), Color({ r, g, b, a }), SpriteIndex(spriteIndex)
		{
		}
	};

	struct Vertex3D {
	public:
		glm::vec3 Position;
		glm::vec3 UV;
		glm::vec4 Color;

		Vertex3D()
			: Position(0.f, 0.f, 0.f), UV(0.f, 0.f, 0.0f), Color(0.f, 0.f, 0.f, 0.f)
		{

		}

		Vertex3D(const glm::vec3& position, const glm::vec3& uv, const glm::vec4& color)
			: Position(position), UV(uv), Color(color)
		{
		}

		Vertex3D(float x, float y, float z, float u, float v, float w, float r, float g, float b, float a)
			: Position({ x, y, z }), UV({ u, v, w }), Color({ r, g, b, a })
		{
		}
	};

}
