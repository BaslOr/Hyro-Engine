#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/VertexBuffer.h"

namespace Hyro {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const VertexLayout& layout, uint32_t vertexCountHint);
		~OpenGLVertexBuffer();

		inline VertexLayout GetLayout() const override { return m_Layout; }
		

		void SetData(const std::vector<Vertex>& data) override;

		void Bind() const override;
		void Bind(void* commandBuffer) const override {}

	private:
		uint32_t m_ID;

		VertexLayout m_Layout;
	};

}
