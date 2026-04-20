#pragma once
#include "Platform/Vulkan/VulkanBase.h"

#include "VulkanDevice.h"
#include "VulkanSurface.h"

namespace Hyro {

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;	
	};

	class VulkanSwapchain {
	public:
		VulkanSwapchain(const Ref<VulkanDevice> device, const Ref<VulkanSurface> surface);
		~VulkanSwapchain();

	private:
		void CreateSwapchain();
		void CreateImageViews();

		void Recreate();

		SwapchainSupportDetails GetSwapchainSupportDetails();

		VkSurfaceFormatKHR ChooseFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	private:
		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		VkSurfaceFormatKHR m_Format;
		VkPresentModeKHR m_PresentMode;
		VkExtent2D m_Extent;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;

	};

}