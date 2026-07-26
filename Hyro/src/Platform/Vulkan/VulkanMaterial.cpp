#include "pch.h"
#include "Platform/Vulkan/VulkanMaterial.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanTexture.h"


#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	VulkanMaterial::VulkanMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{
		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		m_DescriptorSets.resize(maxFramesInFlight);

		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader.get());
		m_DescriptorSets = VulkanDescriptorPool::AllocateDescriptorSets(vulkanShader->GetVkDescriptorSetLayout(), maxFramesInFlight);

		m_FallbackTexture = AssetManager::GetFallbackTexture();
	}

	void VulkanMaterial::SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer)
	{
		m_UniformBuffers[uniformBuffer->GetBinding()] = uniformBuffer;
		m_IsDirty = true;
	}

	void VulkanMaterial::SetTextures(const std::array<Ref<Texture>, 16>& textures)
	{
		m_Textures[0] = m_FallbackTexture;
		for (size_t i = 1; i < textures.size(); ++i) {
			if (textures[i] != nullptr)
				m_Textures[i] = textures[i];
			else
				m_Textures[i] = m_FallbackTexture;
		}
		m_IsDirty = true;
	}

	void VulkanMaterial::SetPushConstantBlock(const PushConstantBlock& pushConstants)
	{
		m_PushConstantBlocks.push_back(pushConstants);
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


		for (auto& block : m_PushConstantBlocks) {
			std::vector<uint8_t> data;
			data.reserve(block.Size);
			for (const auto& uniform : block.GetUniforms()) {
				memcpy(data.data()+data.size(), uniform.Data, SizeOfDescriptorType(uniform.Type));
			}

			vkCmdPushConstants((VkCommandBuffer)commandBuffer, vulkanShader->GetVkPipelineLayout(),
				VK_SHADER_STAGE_VERTEX_BIT,
				0,
				block.Size, data.data());
		}

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
				VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(m_Textures.at(j).get());

				imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfos[j].imageView = vulkanTexture->GetVkImageView();
				imageInfos[j].sampler = vulkanTexture->GetVkSampler();
			}

			//Retrieve Reflection Data
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
