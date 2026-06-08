#include "pch.h"
#include "VulkanAPI.h"

namespace Hyro {

	VulkanAPI::VulkanAPI(const GraphicsPipelineSettings& settings)
	{
		m_Instance = CreateRef<VulkanInstance>();
		m_Surface = CreateRef<VulkanSurface>(m_Instance);
		m_Device = CreateRef<VulkanDevice>(m_Instance, m_Surface);
		m_Swapchain = CreateRef<VulkanSwapchain>(m_Device, m_Surface);
		m_GraphicsPipeline = CreateRef<VulkanGraphicsPipeline>(m_Device, settings);
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