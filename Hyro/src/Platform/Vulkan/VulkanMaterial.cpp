#include "pch.h"
#include "Platform/Vulkan/VulkanMaterial.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanTexture.h"
#include "Platform/Vulkan/VulkanCubemap.h"


#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	VulkanMaterial::VulkanMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_ReflectionData = m_Shader->GetReflectionData();

		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		m_DescriptorSets.resize(maxFramesInFlight);
		m_PushConstantBlocks.reserve(m_ReflectionData.PushConstants.size());

		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader.get());
		m_DescriptorSets = VulkanDescriptorPool::AllocateDescriptorSets(vulkanShader->GetVkDescriptorSetLayout(), maxFramesInFlight);

		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			if (descriptor.Type == DescriptorType::Sampler) {
				m_Textures.resize(descriptor.Count);
				for (size_t i = 0; i < m_Textures.size(); ++i) {
					m_Textures[i] = m_FallbackTexture;
				}
			}
		}

		m_FallbackTexture = AssetManager::GetFallbackTexture();
	}

	void VulkanMaterial::SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer)
	{
		m_UniformBuffers[uniformBuffer->GetBinding()] = uniformBuffer;
		m_IsDirty = true;
	}

	void VulkanMaterial::SetSamplers(const std::array<Ref<Texture>, 16>& textures)
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

	void VulkanMaterial::SetSamplerCube(const Ref<Cubemap>& cubemap)
	{
		m_Cubemap = cubemap;
		m_IsDirty = true;
	}

	void VulkanMaterial::SetPushConstantBlock(const PushConstantBlock& block)
	{
		for (auto& pushConstantBlock : m_PushConstantBlocks) {
			if (pushConstantBlock.Name.compare(block.Name) == 0) {
				pushConstantBlock = block;
				return;
			}
		}

		for (auto& uniform : block.GetUniforms())
		{
			m_PushConstantBlocks.emplace_back(block);
		}
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

		std::vector<VkWriteDescriptorSet> writes;
		writes.resize(m_ReflectionData.Descriptors.size());


		std::vector<VkDescriptorImageInfo> imageInfos(m_Textures.size());
		VkDescriptorBufferInfo bufferInfo{};

		//Descriptor sets for each frame in flight
		for (uint32_t i = 0; i < maxFramesInFlight; i++)
		{
			//Descriptor write for each descriptor/uniform
			for (uint32_t descriptorIndex = 0; descriptorIndex < m_ReflectionData.Descriptors.size(); ++descriptorIndex) {
				auto& descriptor = m_ReflectionData.Descriptors[descriptorIndex];

				writes[descriptorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writes[descriptorIndex].dstSet = m_DescriptorSets[i];
				writes[descriptorIndex].dstBinding = descriptor.Binding;
				writes[descriptorIndex].dstArrayElement = 0;
				writes[descriptorIndex].descriptorType = VulkanShader::HyroDescriptorTypeToVulkanType(descriptor.Type);
				writes[descriptorIndex].descriptorCount = descriptor.Count;
				if (descriptor.Type == DescriptorType::UniformBuffer)
				{
					VulkanUniformBuffer* vulkanUBO = static_cast<VulkanUniformBuffer*>(m_UniformBuffers.at(0).get());

					bufferInfo.buffer = vulkanUBO->GetBufferAtIndex(i);
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UniformBufferData);

					writes[descriptorIndex].pBufferInfo = &bufferInfo;
				}
				else if (descriptor.Type == DescriptorType::Sampler) {

					for (size_t j = 0; j < imageInfos.size(); j++)
					{
						if (imageInfos.size() > 1) {
							VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(m_Textures[j].get());

							imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
							imageInfos[j].imageView = vulkanTexture->GetVkImageView();
							imageInfos[j].sampler = vulkanTexture->GetVkSampler();
						}
						else {
							VulkanCubemap* vulkanCubemap = static_cast<VulkanCubemap*>(m_Cubemap.get());

							imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
							imageInfos[j].imageView = vulkanCubemap->GetVkImageView();
							imageInfos[j].sampler = vulkanCubemap->GetVkSampler();
						}
					}

					writes[descriptorIndex].pImageInfo = imageInfos.data();
				}
			}
			vkUpdateDescriptorSets(VulkanDevice::GetVkDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
		}

		m_IsDirty = false;
	}

}
