#include "pch.h"
#include "Platform/Vulkan/VulkanAPI.h"

#include "Platform/Vulkan/VulkanContext.h"

namespace Hyro {

	VulkanAPI::VulkanAPI(const GraphicsPipelineSettings& settings)
	{
		m_Pipeline = CreateScope<VulkanGraphicsPipeline>(settings);
		VulkanCommandPool::Init();
	}

	void VulkanAPI::DrawIndexed(uint32_t count)
	{

	}

	void VulkanAPI::Clear()
	{
	}

	void VulkanAPI::SetClearColor(const glm::vec4& color)
	{
	}

}
