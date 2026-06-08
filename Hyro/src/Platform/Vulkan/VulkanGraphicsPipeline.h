#pragma once
#include "Platform/Vulkan/VulkanDevice.h"

#include "Hyro/Renderer/Renderer.h"

namespace Hyro {

	class VulkanGraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const Ref<VulkanDevice> device, const GraphicsPipelineSettings& settings);
		~VulkanGraphicsPipeline();

	private:
		std::vector<char> ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;

		Ref<VulkanDevice> m_Device;
	};

}