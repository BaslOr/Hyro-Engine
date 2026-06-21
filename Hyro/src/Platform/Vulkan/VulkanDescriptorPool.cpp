#include "pch.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanContext.h"

#include "Platform/Vulkan/VulkanDevice.h"

#include <backends/imgui_impl_vulkan.h>


namespace Hyro {

	void VulkanDescriptorPool::Init()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes = { {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          100 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  100 },  // ← ImGui!
		} };

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // optional aber praktisch
		poolInfo.maxSets = 200;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();

		if (vkCreateDescriptorPool(VulkanDevice::GetVkDevice(), &poolInfo, g_VulkanAllocationCallback, &s_Pool) != VK_SUCCESS) {
			HYRO_LOG_CORE_FATAL("Failed to Create Vulkan Descriptor Pool!");
		}
	}

	void VulkanDescriptorPool::Destroy()
	{
		vkDestroyDescriptorPool(VulkanDevice::GetVkDevice(), s_Pool, g_VulkanAllocationCallback);
	}

	std::vector<VkDescriptorSet> VulkanDescriptorPool::AllocateDescriptorSets(VkDescriptorSetLayout layout , uint32_t count)
	{
		std::vector<VkDescriptorSet> sets;
		sets.resize(count);
		std::vector<VkDescriptorSetLayout> layouts(count, layout);

		//std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = s_Pool;
		allocInfo.descriptorSetCount = count;
		allocInfo.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(VulkanDevice::GetVkDevice(), &allocInfo, sets.data()) != VK_SUCCESS) {
			HYRO_LOG_CORE_ERROR("Failed to Allocate Descriptor Set!");
		}

		return sets;
	}

	void VulkanDescriptorPool::FreeDescriptorSet(const std::vector<VkDescriptorSet>& descriptorSets)
	{
		vkFreeDescriptorSets(VulkanDevice::GetVkDevice(), s_Pool, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data());
	}

}
