#pragma once
#include <string>

namespace Hyro {

	struct GraphicsPipelineSettings {
		bool EnableBlending = true;
		uint32_t SampleCount = 4;
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
	};

}