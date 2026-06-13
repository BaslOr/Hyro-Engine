#include "pch.h"
#include "VulkanCommandPool.h"

namespace Hyro {

	void VulkanCommandPool::Init()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = VulkanDevice::GetQueueFamilyIndices().GraphcisQueueFamily.value();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(VulkanDevice::GetVkDevice(), &poolInfo, g_VulkanAllocationCallback, &m_CommandPool) != VK_SUCCESS) {
			HYRO_LOG_CORE_FATAL("Failed to create command pool!");
		}
	}

	std::vector<VkCommandBuffer> VulkanCommandPool::AllocateCommandBuffers(uint32_t count, VkCommandBufferLevel level)
	{
		std::vector<VkCommandBuffer> commandBuffers(count);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = level;
		allocInfo.commandBufferCount = count;

		if (vkAllocateCommandBuffers(VulkanDevice::GetVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {	
			HYRO_LOG_CORE_FATAL("Failed to allocate command buffer!");
		}

		return	commandBuffers;
	}

	void VulkanCommandPool::FreeCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers)
	{
		vkFreeCommandBuffers(VulkanDevice::GetVkDevice(), m_CommandPool, commandBuffers.size(), commandBuffers.data());
	}

	VkCommandBuffer VulkanCommandPool::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(VulkanDevice::GetVkDevice(), &allocInfo, &commandBuffer);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanCommandPool::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(VulkanDevice::GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(VulkanDevice::GetGraphicsQueue());

		vkFreeCommandBuffers(VulkanDevice::GetVkDevice(), m_CommandPool, 1, &commandBuffer);
	}

}
