#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class VulkanTexture : public Texture {
	public:
		VulkanTexture(const std::string& filePath);
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
		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspect);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);


	private:
		void CreateSampler();


	private:
		VkImage m_Texture;
		VkDeviceMemory m_Memory;
		VkImageView m_View;
		VkSampler m_Sampler;
	};

}
