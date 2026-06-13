#pragma once
#include "Hyro/Renderer/GraphicsPipeline.h"

#include <vulkan/vulkan.h>


namespace Hyro {

	class VulkanGraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineSettings& settings);
		~VulkanGraphicsPipeline();

		inline VkPipeline GetVkPipeline() const { return m_Pipeline; }

	private:

		std::vector<char> ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;
	};

}