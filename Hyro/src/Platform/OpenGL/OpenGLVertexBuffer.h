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

		void SetData(const std::vector<Vertex>& data) override;

		void Bind() const override;
		void UnBind() const override;
	private:
		uint32_t m_ID;
	};

}
