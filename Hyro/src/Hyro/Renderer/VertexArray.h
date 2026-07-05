#pragma once
#include "Hyro/Core/Memory.h"	
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

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

	class VertexArray {
	public:
		enum TYPE {
			FLOAT
		};

	public:
		static Ref<VertexArray> Create();

		virtual void AddVertexBuffer(Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> buffer) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;//Onyl for Vulkan

		virtual void SetLayout(const VertexLayout& layout, Ref<VertexBuffer> vertexBuffer) = 0;

	};

	template<VertexAttributeType T>
	inline void VertexLayout::Push()
	{
		uint32_t size = GetVertexAttributeSize(T);
		m_Stride += size;
		m_VertexAttributes.push_back(T);
	}

}
