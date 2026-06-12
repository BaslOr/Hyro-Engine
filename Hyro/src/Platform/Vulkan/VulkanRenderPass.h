#pragma once
#include "Platform/Vulkan/VulkanDevice.h"
#include "Hyro/Core/Memory.h"

#include "Platform/Vulkan/VulkanSwapchain.h"


namespace Hyro {

	class VulkanRenderPass {
	public:
		VulkanRenderPass(const Ref<VulkanSwapchain> swapchain);
		~VulkanRenderPass();

		VkRenderPass GetRenderPass() const { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;
	};

}