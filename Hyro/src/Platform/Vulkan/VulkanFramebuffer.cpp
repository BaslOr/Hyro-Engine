#include "pch.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanContext.h"


namespace Hyro {

	VulkanFramebuffer::VulkanFramebuffer(const Ref<VulkanSwapchain> swapchain)
	{
		m_Framebuffers.resize(swapchain->GetImageViews().size());

		for (size_t i = 0; i < swapchain->GetImageViews().size(); i++) {
			VkImageView attachments[] = {
				swapchain->GetImageViews()[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = VulkanContext::Get().GetRenderPass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapchain->GetExtent().width;
			framebufferInfo.height = swapchain->GetExtent().height;
			framebufferInfo.layers = 1;
			if (vkCreateFramebuffer(m_Device->GetVkDevice(), &framebufferInfo, g_VulkanAllocationCallback, &m_Framebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		for (size_t i = 0; i < m_Framebuffers.size(); i++) {
			vkDestroyFramebuffer(m_Device->GetVkDevice(), m_Framebuffers[i], g_VulkanAllocationCallback);
		}
	}

}
