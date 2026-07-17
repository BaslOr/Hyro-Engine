#pragma once
#include "Hyro/Renderer/RenderingObjects/VertexArray.h"

#include <cstdint>

namespace Hyro {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void Bind(void* commandBuffer) const override {}

		void AddVertexBuffer(Ref<VertexBuffer> buffer) override;
		void SetIndexBuffer(Ref<IndexBuffer> buffer) override;

	private:
		int AttributeTypeToOpenGLEnum(ShaderType type) const;

	private:
		uint32_t m_ID;
	};
	
}