#pragma once
#include "Hyro/Core/Memory.h"

#include <glm/glm.hpp>

namespace Hyro {


	struct UniformBufferData {
		glm::mat4 MVP;
	};

	class UniformBuffer {
	public:
		static Ref<UniformBuffer> Create();

		virtual void SetData(const UniformBufferData& ubo) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer, void* pipelineLayout) const = 0;
	};

}
