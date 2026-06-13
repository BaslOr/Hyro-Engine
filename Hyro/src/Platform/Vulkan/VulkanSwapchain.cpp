#include "pch.h"
#include "VulkanSwapchain.h"

#include "Hyro/Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"


namespace Hyro {

	VulkanSwapchain::VulkanSwapchain(const Ref<VulkanSurface> surface)
		: m_Surface(surface)
	{
		CreateSwapchain();
		CreateImageViews();
		CreateRenderPass();
		CreateFramebuffers();

		HYRO_LOG_CORE_TRACE("Created Vulkan Swapchain");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		CleanUpOldSwapchain();
		vkDestroyRenderPass(VulkanDevice::GetVkDevice(), m_RenderPass, g_VulkanAllocationCallback);
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		VkDevice device = VulkanDevice::GetVkDevice();

		SwapchainSupportDetails details = GetSwapchainSupportDetails();
		VkSurfaceFormatKHR surfaceFormat = ChooseFormat(details.formats);
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
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = m_Extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = VulkanDevice::GetQueueFamilyIndices();
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

		VkCheck(vkCreateSwapchainKHR(device, &swapchainInfo, g_VulkanAllocationCallback, &m_Swapchain));

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, m_Images.data());

		m_Format = surfaceFormat.format;
	}

	void VulkanSwapchain::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_Format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkCheck(vkCreateImageView(VulkanDevice::GetVkDevice(), &createInfo, g_VulkanAllocationCallback, &m_ImageViews[i]));
		}
	}

	void VulkanSwapchain::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_Format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(VulkanDevice::GetVkDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void VulkanSwapchain::CreateFramebuffers()
	{
		m_Framebuffers.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageView attachments[] = {
				m_ImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_Extent.width;
			framebufferInfo.height = m_Extent.height;
			framebufferInfo.layers = 1;
			if (vkCreateFramebuffer(VulkanDevice::GetVkDevice(), &framebufferInfo, g_VulkanAllocationCallback, &m_Framebuffers[i]) != VK_SUCCESS) {
				HYRO_LOG_CORE_FATAL("Failed to create framebuffers!");
			}
		}
	}

	void VulkanSwapchain::CleanUpOldSwapchain()
	{
		vkDestroySwapchainKHR(VulkanDevice::GetVkDevice(), m_Swapchain, g_VulkanAllocationCallback);

		for (auto imageView : m_ImageViews) {
			vkDestroyImageView(VulkanDevice::GetVkDevice(), imageView, g_VulkanAllocationCallback);
		}

		for (auto framebuffer : m_Framebuffers) {
			vkDestroyFramebuffer(VulkanDevice::GetVkDevice(), framebuffer, g_VulkanAllocationCallback);
		}
	}

	void VulkanSwapchain::Recreate(uint32_t width, uint32_t height)
	{
		CleanUpOldSwapchain();

		CreateSwapchain();
		CreateImageViews();
		CreateFramebuffers();
	}

	SwapchainSupportDetails VulkanSwapchain::GetSwapchainSupportDetails()
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanDevice::GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDevice::GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDevice::GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDevice::GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDevice::GetVkPhysicalDevice(), m_Surface->GetVkSurface(), &presentModeCount, details.presentModes.data());
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