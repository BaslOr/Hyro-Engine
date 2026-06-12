#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanRenderPass.h"

#include <vector>

namespace Hyro {

	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(const Ref<VulkanSwapchain> swapchain);
		~VulkanFramebuffer();

		VkFramebuffer GetVkFramebuffer(size_t index) const { return m_Framebuffers[index]; }

	private:
		Ref<VulkanDevice> m_Device;

		std::vector<VkFramebuffer> m_Framebuffers;
	};

}