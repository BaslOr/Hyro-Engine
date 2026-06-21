#pragma once
#include "Hyro/Renderer/UniformBuffer.h"

namespace Hyro {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer() = default;
		~OpenGLUniformBuffer() = default;

		void SetData(const UniformBufferData& ubo) override {}

		void Bind() const override {}
		void Bind(void* commandBuffer, void* pipelineLayout) const override {}

	private:

	};

}