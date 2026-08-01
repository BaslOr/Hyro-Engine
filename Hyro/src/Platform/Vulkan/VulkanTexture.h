#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include <vulkan/vulkan_core.h>

namespace Hyro {

	class VulkanTexture : public Texture {
	public:
		VulkanTexture(const std::string& filePath);
		VulkanTexture(VkImage image, VkImageView imageView, VkSampler sampler);
		~VulkanTexture();


		void Bind(uint32_t slot) const override;
		inline void Bind(void* commandBuffer) const override {}

		inline VkImageView GetVkImageView() const { return m_View; }
		inline VkSampler GetVkSampler() const { return m_Sampler; }

		static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		static void CreateImage(uint32_t width, uint32_t height,
			VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
			VkImage& image, VkDeviceMemory& imageMemory);
		static void TransitionImageLayout(VkImage image, VkFormat format,
			VkImageLayout oldLayout, VkImageLayout newLayout,
			VkImageAspectFlags aspect, uint32_t layerCount = 1);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount = 1, uint32_t layerSize = 1);
		static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1);
		static VkSampler CreateSampler(VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipMode);
	private:
		VkImage m_Texture;
		VkDeviceMemory m_Memory;
		VkImageView m_View;
		VkSampler m_Sampler;
	};

}
