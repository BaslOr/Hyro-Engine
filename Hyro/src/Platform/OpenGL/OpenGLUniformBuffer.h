#pragma once
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

namespace Hyro {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer(uint32_t binding, uint32_t size);
		~OpenGLUniformBuffer();

		void SetData(void* data) override;

		void Bind() const override;
		void Bind(void* commandBuffer, void* pipelineLayout) const override;

	private:
		uint32_t m_Buffer;
		uint32_t m_Binding;
		uint32_t m_Size;
	};

}