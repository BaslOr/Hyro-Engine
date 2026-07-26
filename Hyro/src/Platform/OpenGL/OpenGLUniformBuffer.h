#pragma once
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

namespace Hyro {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer();
		~OpenGLUniformBuffer();

		void SetData(const UniformBufferData& ubo) override;

		void Bind() const override;
		void Bind(void* commandBuffer, void* pipelineLayout) const override;

		uint32_t GetBinding() const { return m_Binding; }

	private:
		uint32_t m_Buffer;
		uint32_t m_Binding;
	};

}