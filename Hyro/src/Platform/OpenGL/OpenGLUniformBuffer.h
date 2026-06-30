#pragma once
#include "Hyro/Renderer/UniformBuffer.h"

namespace Hyro {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer();
		~OpenGLUniformBuffer();

		void SetData(const UniformBufferData& ubo) override;

		void Bind() const override;
		void Bind(void* commandBuffer, void* pipelineLayout) const override;

	private:
		uint32_t m_Buffer;
	};

}