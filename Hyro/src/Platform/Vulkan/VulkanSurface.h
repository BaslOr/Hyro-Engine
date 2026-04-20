#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "VulkanInstance.h"

namespace Hyro {

	class VulkanSurface {
	public:
		VulkanSurface(const Ref<VulkanInstance> instance);
		~VulkanSurface();

		inline VkSurfaceKHR GetVkSurface() const { return m_Surface; }

	private:
		Ref<VulkanInstance> m_Instance;

		VkSurfaceKHR m_Surface;
	};

}