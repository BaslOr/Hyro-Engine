#include "pch.h"
#include "VulkanSwapchain.h"

#include "Hyro/Core/Application.h"


namespace Hyro {

	VulkanSwapchain::VulkanSwapchain(const Ref<VulkanDevice> device, const Ref<VulkanSurface> surface)
		: m_Device(device), m_Surface(surface)
	{
		CreateSwapchain();
		CreateImageViews();

		HYRO_LOG_CORE_TRACE("Created Vulkan Swapchain");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(m_Device->GetDevice(), m_Swapchain, g_VulkanAllocationCallback);
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		SwapchainSupportDetails details = GetSwapchainSupportDetails();
		m_Format = ChooseFormat(details.formats);
		m_PresentMode = ChoosePresentMode(details.presentModes);
		m_Extent = ChooseExtent(details.capabilities);

		uint32_t imageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
			imageCount = details.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_Surface->GetVkSurface();
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = m_Format.format;
		swapchainInfo.imageColorSpace = m_Format.colorSpace;
		swapchainInfo.imageExtent = m_Extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = m_Device->GetQueueFamilyIndices();;
		uint32_t queueFamilyIndices[] = { indices.GraphcisQueueFamily.value(), indices.PresentationQueueFamily.value() };

		if (indices.GraphcisQueueFamily != indices.PresentationQueueFamily) {
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0; // Optional
			swapchainInfo.pQueueFamilyIndices = nullptr; // Optional
		}
		swapchainInfo.preTransform = details.capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = m_PresentMode;
		swapchainInfo.clipped = VK_FALSE;// In case of taking screenshots, we want to be able to read the pixels even if they are not visible on the screen
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		VkCheck(vkCreateSwapchainKHR(m_Device->GetDevice(), &swapchainInfo, g_VulkanAllocationCallback, &m_Swapchain));

		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_Swapchain, &imageCount, m_Images.data());
	}

	void VulkanSwapchain::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_Format.format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkCheck(vkCreateImageView(m_Device->GetDevice(), &createInfo, g_VulkanAllocationCallback, &m_ImageViews[i]));
		}
	}

	void VulkanSwapchain::Recreate()
	{
	}

	SwapchainSupportDetails VulkanSwapchain::GetSwapchainSupportDetails()
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &presentModeCount, details.presentModes.data());
		}

		if (details.formats.empty() || details.presentModes.empty()) {
			HYRO_LOG_CORE_FATAL("Graphics card does not support Vulkan presentation!");
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& format : availableFormats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& presentMode : availablePresentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			const auto window = Application::Get().GetWindow()->GetNative();
			glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

}