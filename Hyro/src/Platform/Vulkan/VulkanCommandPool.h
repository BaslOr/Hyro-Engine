#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Platform/Vulkan/VulkanDevice.h"

namespace Hyro {

	class VulkanCommandPool {
	public:		
		static void Init();

		static VkCommandBuffer AllocateCommandBuffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		static VkCommandBuffer BeginSingleTimeCommands();
		static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		static VkCommandPool GetVkCommandPool() { return m_CommandPool; }

	private:
		static inline VkCommandPool m_CommandPool;

	};

}