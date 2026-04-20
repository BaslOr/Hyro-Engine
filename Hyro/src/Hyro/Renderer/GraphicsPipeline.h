#pragma once
#include "Hyro/Core/Core.h"

namespace Hyro {

	struct GraphicsPipelineSettings {
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
		bool EnableBlending = true;
		uint32_t SampleCount = 1;
	};

	class GraphicsPipeline {
	public:
		static Ref<GraphicsPipeline> Create(const GraphicsPipelineSettings& settings);

	private:

	};

}