#pragma once
#include <string>
#include <cstdint>

namespace Hyro {

	struct GraphicsPipelineSettings {
		bool EnableBlending = true;
		uint32_t SampleCount = 4;
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
	};

}