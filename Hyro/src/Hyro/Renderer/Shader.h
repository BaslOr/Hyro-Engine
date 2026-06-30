#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsPipeline.h"

namespace Hyro {

	class Shader {
	public:
		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		static Ref<Shader> Create(const GraphicsPipelineSettings& settings);
	};

}
