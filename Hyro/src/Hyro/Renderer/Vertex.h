#pragma once
#include <glm/glm.hpp>
#include <string>


namespace Hyro {

	enum class ShaderType {
		NONE,
		FLOAT, FLOAT2, FLOAT3, FLOAT4
	};

	class VertexLayout {
	public:
		//Elements are stored in order of location
		struct Element {
			std::string Name;
			ShaderType Type;
			uint32_t Offset;
		};

		void Push(const std::string& name, ShaderType type);

		inline std::vector<Element> GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		uint32_t GetVertexAttributeSize(ShaderType type) const;

	private:
		std::vector<Element> m_Elements;
		uint32_t m_Stride = 0;
	};


	inline void VertexLayout::Push(const std::string& name, ShaderType type)
	{
		uint32_t size = GetVertexAttributeSize(type);
		uint32_t offset = m_Stride;
		m_Stride += size;

		m_Elements.push_back({ name, type, offset });
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

}
