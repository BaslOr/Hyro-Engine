#include "pch.h"
#include "Platform/Vulkan/VulkanCubemap.h"
#include "Platform/Vulkan/VulkanTexture.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanDevice.h"

#include "Hyro/Utils/Bitmap.h"

#include <stb_image.h>
#include <stb_image_write.h>

namespace Hyro {

	VulkanCubemap::VulkanCubemap(const std::string& filePath)
	{
		int width, height;
		float* data;
		data = stbi_loadf(filePath.c_str(), &width, &height, nullptr, 4);
		if (!data) {
			HYRO_LOG_CORE_ERROR("Failed to load cubemap from file! Path: {0}", filePath.c_str());
			return;
		}
		Bitmap in(width, height, 4, BitmapFormat::Float, BitmapType::TwoD, data);

		Bitmap out = ConvertEquirectangularMapToVerticalCross(in);
		stbi_image_free(data);
		stbi_write_hdr(".cache/screenshot.hdr", out.GetWidth(), out.GetHeight(), out.GetComponents(), (const float*)out.GetData());

		Bitmap cubemap = ConvertVerticalCrossToCubeMapFaces(out);
		cubemap.ChangeComponents(4);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		const uint32_t pixelSize = cubemap.GetComponents() * Bitmap::GetBytesPerComponent(cubemap.GetFormat());
		const uint32_t imageSize = cubemap.GetWidth() * cubemap.GetHeight() * pixelSize * 6;

		VulkanBuffer::CreateBufer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingMemory);

		void* mappedData;
		vkMapMemory(VulkanDevice::GetVkDevice(), stagingMemory, 0, imageSize, 0, &mappedData);
		memcpy(mappedData, cubemap.GetData(), imageSize);
		vkUnmapMemory(VulkanDevice::GetVkDevice(), stagingMemory);

		CreateCubemapImage(cubemap.GetWidth(), cubemap.GetHeight());

		VulkanTexture::TransitionImageLayout(m_CubemapImage, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, 6);
		VulkanTexture::CopyBufferToImage(stagingBuffer, m_CubemapImage, static_cast<uint32_t>(cubemap.GetWidth()), static_cast<uint32_t>(cubemap.GetHeight()), 6, (imageSize/6));
		VulkanTexture::TransitionImageLayout(m_CubemapImage, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, 6);
		
		m_ImageView = VulkanTexture::CreateImageView(m_CubemapImage, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE, 6);
		m_Sampler = VulkanTexture::CreateSampler(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_MIPMAP_MODE_LINEAR);
	}

	VulkanCubemap::~VulkanCubemap()
	{
		VkDevice device = VulkanDevice::GetVkDevice();
		vkFreeMemory(device, m_ImageMemory, g_VulkanAllocationCallback);
		vkDestroySampler(device, m_Sampler, g_VulkanAllocationCallback);
		vkDestroyImageView(device, m_ImageView, g_VulkanAllocationCallback);
		vkDestroyImage(device, m_CubemapImage, g_VulkanAllocationCallback);
	}

	void VulkanCubemap::Bind() const
	{
		HYRO_LOG_CORE_WARN("Tried to bind Vulkan Cubemap without command buffer. This may indicate a bug.");
	}

	void VulkanCubemap::Unbind() const
	{
		HYRO_LOG_CORE_WARN("Tried to unbind Vulkan Cubemap without command buffer. This may indicate a bug.");
	}

	void VulkanCubemap::CreateCubemapImage(uint32_t width, uint32_t height)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 6;
		imageInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		if (vkCreateImage(VulkanDevice::GetVkDevice(), &imageInfo, nullptr, &m_CubemapImage) != VK_SUCCESS) {
			HYRO_LOG_CORE_ERROR("Failed to Create Cubemap Image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(VulkanDevice::GetVkDevice(), m_CubemapImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanBuffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(VulkanDevice::GetVkDevice(), &allocInfo, g_VulkanAllocationCallback, &m_ImageMemory) != VK_SUCCESS) {
			HYRO_LOG_CORE_ERROR("Failed to Allocate Image Memory!");
		}

		vkBindImageMemory(VulkanDevice::GetVkDevice(), m_CubemapImage, m_ImageMemory, 0);
	}

}
