#pragma once
#include "Platform/Vulkan/VulkanBase.h"

namespace Hyro {
	
	class VulkanDescriptorPool {
	public:
		static void Init();
		static void Destroy();

		static std::vector<VkDescriptorSet> AllocateDescriptorSets(VkDescriptorSetLayout layout, uint32_t count);
		static void FreeDescriptorSet(const std::vector<VkDescriptorSet>& descriptorSets);

		static VkDescriptorPool GetVkPool() { return s_Pool; }


	private:
		static inline VkDescriptorPool s_Pool;
	};

}