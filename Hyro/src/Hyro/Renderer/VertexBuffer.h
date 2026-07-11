#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {


	enum class VertexAttributeType {
		FLOAT, FLOAT2, FLOAT3, FLOAT4
	};

	class VertexLayout {
	public:
		template<VertexAttributeType T> void Push();

		inline const std::vector<VertexAttributeType>& GetVertexAttributes() const { return m_VertexAttributes; }
		inline uint32_t GetStride() const { return m_Stride; }

		uint32_t GetVertexAttributeSize(VertexAttributeType type) const;

	private:
		std::vector<VertexAttributeType> m_VertexAttributes;
		uint32_t m_Stride;
	};


	template<VertexAttributeType T>
	inline void VertexLayout::Push()
	{
		uint32_t size = GetVertexAttributeSize(T);
		m_Stride += size;
		m_VertexAttributes.push_back(T);
	}


	class VertexBuffer {
	public:
		virtual void SetData(const std::vector<Vertex>& data) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual void SetLayout(const VertexLayout& layout) = 0;
		virtual VertexLayout GetLayout() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const std::vector<Vertex>& vertices);
	};

}
