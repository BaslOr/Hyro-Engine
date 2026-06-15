#include "pch.h"
#include "Platform/Vulkan/VulkanBuffer.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCommandPool.h"

namespace Hyro {
	
	//Utility Functions for Vulkan Buffers
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanDevice::GetVkPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		HYRO_ASSERT(false);
	}


	static void CreateBufer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(VulkanDevice::GetVkDevice(), &bufferInfo, g_VulkanAllocationCallback, &buffer) != VK_SUCCESS) {
			HYRO_LOG_CORE_ERROR("Failed to create Vertex Buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(VulkanDevice::GetVkDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(VulkanDevice::GetVkDevice(), &allocInfo, g_VulkanAllocationCallback, &bufferMemory) != VK_SUCCESS) {
			HYRO_LOG_CORE_ERROR("Failed to allocate Vertex Buffer Memory!");
		}

		vkBindBufferMemory(VulkanDevice::GetVkDevice(), buffer, bufferMemory, 0);
	}

	static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = VulkanCommandPool::BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);



		VulkanCommandPool::EndSingleTimeCommands(commandBuffer);
	}

	VulkanVertexBuffer::VulkanVertexBuffer()
	{
		//Query Max Size
		VkDeviceSize bufferSize = 100000;
		CreateBufer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Memory);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Vertex>& vertices)
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		CreateBufer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(VulkanDevice::GetVkDevice(), m_Memory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), bufferSize);
		vkUnmapMemory(VulkanDevice::GetVkDevice(), m_Memory);

		CreateBufer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Memory);

		CopyBuffer(stagingBuffer, m_Buffer, bufferSize);

		vkDestroyBuffer(VulkanDevice::GetVkDevice(), stagingBuffer, g_VulkanAllocationCallback);
		vkFreeMemory(VulkanDevice::GetVkDevice(), stagingMemory, g_VulkanAllocationCallback);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkDestroyBuffer(VulkanDevice::GetVkDevice(), m_Buffer, g_VulkanAllocationCallback);
		vkFreeMemory(VulkanDevice::GetVkDevice(), m_Memory, g_VulkanAllocationCallback);
	}

	void VulkanVertexBuffer::SetData(const std::vector<Vertex>& vertices)
	{
		VkDeviceSize bufferSize = 100000;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		CreateBufer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		//Query Max Size
		void* data;
		vkMapMemory(VulkanDevice::GetVkDevice(), stagingMemory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), bufferSize);
		vkUnmapMemory(VulkanDevice::GetVkDevice(), stagingMemory);

		CopyBuffer(stagingBuffer, m_Buffer, bufferSize);

		vkDestroyBuffer(VulkanDevice::GetVkDevice(), stagingBuffer, g_VulkanAllocationCallback);
		vkFreeMemory(VulkanDevice::GetVkDevice(), stagingMemory, g_VulkanAllocationCallback);
	}

	void VulkanVertexBuffer::Bind(void* commandBuffer) const
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers((VkCommandBuffer)commandBuffer, 0, 1, &m_Buffer, offsets);
	}

	VkVertexInputBindingDescription VulkanVertexBuffer::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription;
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 3> VulkanVertexBuffer::GetAttributeDescription()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, UV);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, Color);

		return attributeDescriptions;
	}

}
