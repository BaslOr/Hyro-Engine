#pragma once
#include "Hyro/Core/Memory.h"

#include <glm/glm.hpp>

namespace Hyro {


	struct TransformData {
		glm::mat4 MVP;
	};

	class UniformBuffer {
	public:

		virtual void SetData(void* data) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer, void* pipelineLayout) const = 0;


	private:
		friend class OpenGLShaderBindings; friend class VulkanShaderBindings; //Uniform buffers should only be created by shaders, as they are bound to a specific binding point in the shader

		static Ref<UniformBuffer> Create(uint32_t binding, uint32_t size);
	};

}
