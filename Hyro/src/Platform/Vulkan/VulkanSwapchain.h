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
		VulkanSwapchain(const Ref<VulkanSurface> surface);
		~VulkanSwapchain();

		inline VkSwapchainKHR GetVkSwapchain() const { return m_Swapchain; }
		inline VkFormat GetImageFormat() const { return m_Format; }
		inline VkExtent2D GetExtent() const { return m_Extent; }
		inline const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }

		inline uint32_t GetMinImageCount() { SwapchainSupportDetails details = GetSwapchainSupportDetails(); return details.capabilities.minImageCount; }

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

		VkFormat m_Format;
		VkPresentModeKHR m_PresentMode;
		VkExtent2D m_Extent;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;

	};

}