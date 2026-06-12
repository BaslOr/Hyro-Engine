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

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(VulkanDevice::GetVkDevice(), &fenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS) {
			HYRO_ASSERT(false, "Failed to create synchronization objects for a frame!");	
		}
	}

	VulkanContext::~VulkanContext()
	{
		vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_ImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_RenderFinishedSemaphore, nullptr);
		vkDestroyFence(VulkanDevice::GetVkDevice(), m_InFlightFence, nullptr);
	}

}
