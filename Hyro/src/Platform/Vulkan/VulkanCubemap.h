#pragma once
#include "Hyro/Renderer/Cubemap.h"

#include "Platform/Vulkan/VulkanBase.h"


#include <string>

namespace Hyro {

	class VulkanCubemap : public Cubemap {
	public:
		VulkanCubemap() = default;
		VulkanCubemap(const std::string& filePath);
		~VulkanCubemap();


		void Bind() const override;
		void Unbind() const override;

		VkImage GetVkImage() const { return m_CubemapImage; }
		VkImageView GetVkImageView() const { return m_ImageView; }
		VkSampler GetVkSampler() const { return m_Sampler; }

	private:
		void CreateCubemapImage(uint32_t width, uint32_t height);

	private:
		VkImage m_CubemapImage;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		VkSampler m_Sampler;
	};

}
