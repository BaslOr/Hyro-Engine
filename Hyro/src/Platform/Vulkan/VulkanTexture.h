#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class VulkanTexture : public Texture {
	public:
		VulkanTexture(const std::string& filePath);
		~VulkanTexture();


		void Bind() const override;
		inline void Bind(void* commandBuffer) const override {}

		inline uint32_t GetSpriteIndex() const override { return m_SpriteIndex; }


		inline VkImageView GetImageView() const { return m_View; }

	private:
		void CreateSampler();

	private:
		VkImage m_Texture;
		VkDeviceMemory m_Memory;
		VkImageView m_View;
		VkSampler m_Sampler;

		uint32_t m_SpriteIndex = 0;
		static inline uint32_t s_NextSpriteIndex = 0;
	};

}
