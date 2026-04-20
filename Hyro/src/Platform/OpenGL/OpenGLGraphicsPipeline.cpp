#include "pch.h"
#include "OpenGLGraphicsPipeline.h"

#include <glad/glad.h>

namespace Hyro {

	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline(const GraphicsPipelineSettings& settings)
	{
		if (settings.EnableBlending) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		if (settings.SampleCount > 1) {
			glEnable(GL_MULTISAMPLE);
		} else {
			glDisable(GL_MULTISAMPLE);
		}
	}

	OpenGLGraphicsPipeline::~OpenGLGraphicsPipeline()
	{
	}

}