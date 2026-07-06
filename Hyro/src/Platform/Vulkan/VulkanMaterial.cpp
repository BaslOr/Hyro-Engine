#include "pch.h"
#include "Platform/Vulkan/VulkanMaterial.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanTexture.h"


namespace Hyro {

	VulkanMaterial::VulkanMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{
		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		m_DescriptorSets.resize(maxFramesInFlight);

		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader.get());
		m_DescriptorSets = VulkanDescriptorPool::AllocateDescriptorSets(vulkanShader->GetVkDescriptorSetLayout(), maxFramesInFlight);
	}

	void VulkanMaterial::SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding)
	{
		m_UniformBuffers[binding] = uniformBuffer;
		m_IsDirty = true;
	}

	void VulkanMaterial::SetTexture(Ref<Texture> texture, uint32_t slot)
	{
		m_Textures[slot] = texture;
		m_IsDirty = true;
	}

	void VulkanMaterial::SetPushConstants(const PushConstants& pushConstants)
	{
		//Sets Push Constants lazily
		m_PushConstants = pushConstants;
	}

	void VulkanMaterial::Bind()
	{
		HYRO_LOG_CORE_ERROR("Tried to bind Material without a CommandBuffer. This may Indicate a Bug.");
	}

	void VulkanMaterial::Bind(void* commandBuffer)
	{
		uint32_t currentFrameIndex = VulkanContext::Get().GetCurrentFrameIndex();
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader.get());

		m_Shader->Bind(commandBuffer);

		vkCmdPushConstants((VkCommandBuffer)commandBuffer, vulkanShader->GetVkPipelineLayout(),
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(PushConstants), &m_PushConstants);

		if (m_IsDirty)
		{
			UpdateDescriptorSets();
		}
		vkCmdBindDescriptorSets((VkCommandBuffer)commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanShader->GetVkPipelineLayout(), 0, 1, &m_DescriptorSets[currentFrameIndex], 0, nullptr);
	}

	void VulkanMaterial::UpdateDescriptorSets()
	{
		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		VulkanUniformBuffer* vulkanUBO = static_cast<VulkanUniformBuffer*>(m_UniformBuffers.at(0).get());

		std::array<VkWriteDescriptorSet, 2> writes{};

		for (size_t i = 0; i < maxFramesInFlight; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = vulkanUBO->GetBufferAtIndex(i);
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferData);

			std::array<VkDescriptorImageInfo, 16> imageInfos;
			for (size_t j = 0; j < imageInfos.size(); j++)
			{
				if (m_Textures.find(j) == m_Textures.end()) {
					Ref<VulkanTexture> fallback = VulkanContext::Get().GetFallbackTexture();

					imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfos[j].imageView = fallback->GetVkImageView();
					imageInfos[j].sampler = fallback->GetVkSampler();
				}
				else {
					VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(m_Textures.at(j).get());

					imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfos[j].imageView = vulkanTexture->GetVkImageView();
					imageInfos[j].sampler = vulkanTexture->GetVkSampler();
				}
			}

			writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[0].dstSet = m_DescriptorSets[i];
			writes[0].dstBinding = 0;
			writes[0].dstArrayElement = 0;
			writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[0].descriptorCount = 1;
			writes[0].pBufferInfo = &bufferInfo;
			writes[0].pImageInfo = nullptr; // Optional
			writes[0].pTexelBufferView = nullptr; // Optional

			writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[1].dstSet = m_DescriptorSets[i];
			writes[1].dstBinding = 1;
			writes[1].dstArrayElement = 0;
			writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writes[1].descriptorCount = 16;
			writes[1].pImageInfo = imageInfos.data();


			vkUpdateDescriptorSets(VulkanDevice::GetVkDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
		}

		m_IsDirty = false;
	}

}
