#pragma once
#include "pch.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanCommandPool.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"

namespace Hyro {

	VulkanContext::VulkanContext(void* windowHandle)
	{
		s_Instance = this;

		m_Instance = CreateRef<VulkanInstance>();
		m_Surface = CreateRef<VulkanSurface>(m_Instance, windowHandle);
		VulkanDevice::Init(m_Instance, m_Surface);
		VulkanCommandPool::Init();
		m_Swapchain = CreateRef<VulkanSwapchain>(m_Surface);
		VulkanDescriptorPool::Init();

		//TODO: Load from AssetManager
		m_FallbackTexture = CreateRef<VulkanTexture>("Assets/Textures/Fallback.png");
	}

	VulkanContext::~VulkanContext()
	{
		vkDeviceWaitIdle(VulkanDevice::GetVkDevice());
	}

	void VulkanContext::ResizeViewport(uint32_t width, uint32_t height)
	{
		m_Swapchain->Recreate(width, height);
	}

}
