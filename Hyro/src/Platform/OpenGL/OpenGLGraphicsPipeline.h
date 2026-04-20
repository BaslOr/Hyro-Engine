#pragma once
#include "Hyro/Renderer/GraphicsPipeline.h"

namespace Hyro {

	class OpenGLGraphicsPipeline : public GraphicsPipeline {
	public:
		OpenGLGraphicsPipeline(const GraphicsPipelineSettings& settings);
		~OpenGLGraphicsPipeline();

	private:

	};

}