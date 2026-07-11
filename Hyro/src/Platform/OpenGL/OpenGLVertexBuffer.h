#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/VertexBuffer.h"

namespace Hyro {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer();
		OpenGLVertexBuffer(const std::vector<float>& data);
		OpenGLVertexBuffer(const std::vector<Vertex>& data);
		~OpenGLVertexBuffer();

		inline void SetLayout(const VertexLayout& layout) override { m_Layout = layout; }
		inline VertexLayout GetLayout() const override { return m_Layout; }
		

		void SetData(const std::vector<Vertex>& data) override;

		void Bind() const override;
		void Bind(void* commandBuffer) const override {}

	private:
		uint32_t m_ID;

		VertexLayout m_Layout;
	};

}
