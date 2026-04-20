#pragma once
#include "Hyro/Renderer/GraphicsPipeline.h"


namespace Hyro {

	class VulkanGraphicsPipeline : public GraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineSettings& settings);
		~VulkanGraphicsPipeline();

	private:

	}

}