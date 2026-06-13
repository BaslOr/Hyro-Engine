#pragma once
#include "pch.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Hyro {

	VulkanContext::VulkanContext(void* windowHandle)
	{
		s_Instance = this;

		m_Instance = CreateRef<VulkanInstance>();
		m_Surface = CreateRef<VulkanSurface>(m_Instance, windowHandle);
		VulkanDevice::Init(m_Instance, m_Surface);
		m_Swapchain = CreateRef<VulkanSwapchain>(m_Surface);
		m_RenderPass = CreateRef<VulkanRenderPass>(m_Swapchain);
		m_Framebuffer = CreateScope<VulkanFramebuffer>(m_Swapchain);
	}

	VulkanContext::~VulkanContext()
	{
	}

}
