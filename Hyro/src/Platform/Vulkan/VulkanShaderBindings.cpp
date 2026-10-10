#include "pch.h"
#include "Platform/Vulkan/VulkanShaderBindings.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanTexture.h"
#include "Platform/Vulkan/VulkanCubemap.h"


#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	VulkanShaderBindings::VulkanShaderBindings(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_ReflectionData = m_Shader->GetReflectionData();
		m_FallbackTexture = AssetManager::GetFallbackTexture();

		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		m_DescriptorSets.resize(maxFramesInFlight);
		m_PushConstantBlocks.reserve(m_ReflectionData.PushConstants.size());

		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader.get());
		m_DescriptorSets = VulkanDescriptorPool::AllocateDescriptorSets(vulkanShader->GetVkDescriptorSetLayout(), maxFramesInFlight);

		m_Textures.resize(0);

		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			//Handle Uniform Buffers
			if (descriptor.Type == DescriptorType::UniformBuffer) {
				Ref<UniformBuffer> ubo = UniformBuffer::Create(descriptor.Binding, descriptor.BlockSize);//Technichally binding is not neccesarry to pass but for compatibility reasons(OpenGL)
				m_UniformBuffersByBinding[descriptor.Binding] = ubo;
				m_UniformBuffersByName[descriptor.Name] = ubo;

				m_IsDirty = true;
			}

			//Handle Samplers
			if (descriptor.Type == DescriptorType::Sampler) {
				m_Textures.resize(descriptor.Count);
				for (size_t i = 0; i < m_Textures.size(); ++i) {
					m_Textures[i] = m_FallbackTexture;
				}
				m_IsDirty = true;
			}

			if (descriptor.Type == DescriptorType::SamplerCube) {
				if (descriptor.Count > 1) {
					HYRO_LOG_CORE_ERROR("Shader has a cubemap sampler with count > 1. This is not supported. This may indicate a bug.");
				}

				m_HasCubemapSampler = true;
			}
		}

		bool hasSamplers = m_Textures.size() > 0;
		if (hasSamplers && m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Shader has both a sampler and a cubemap sampler. This is not supported. This may indicate a bug.");
		}
	}

	Ref<UniformBuffer> VulkanShaderBindings::RetrieveUniformBuffer(const std::string& name) const
	{
		if (m_UniformBuffersByName.find(name) != m_UniformBuffersByName.end())
			return m_UniformBuffersByName[name];

		HYRO_ASSERT(false, "Failed to find Uniform Buffer with name: {}", name.c_str());
		return nullptr;
	}

	uint32_t VulkanShaderBindings::GetNextTextureSlotIndex(Ref<Texture> texture)
	{
		if (texture == nullptr) {
			HYRO_LOG_CORE_ERROR("Tried to get a texture slot for a null texture. This may indicate a bug.");
			return 0;
		}

		if (IsTextureBound(texture))
			return std::distance(m_Textures.begin(), std::find(m_Textures.begin(), m_Textures.end(), texture));

		if (GetFreeTextureSlotCount() == 0) {
			HYRO_LOG_CORE_ERROR("Tried to get a texture slot for a texture but all slots are full. This may indicate a bug.");
			return 0;
		}

		uint32_t slot = 1;
		while (slot < m_Textures.size() && m_Textures[slot] != m_FallbackTexture) {
			slot++;
		}
		m_Textures[slot] = texture;
		m_IsDirty = true;
		return slot;
	}

	uint32_t VulkanShaderBindings::GetFreeTextureSlotCount() const
	{
		uint32_t freeSlots = 0;
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (m_Textures[i] == m_FallbackTexture)
				freeSlots++;
		}

		return freeSlots;
	}

	bool VulkanShaderBindings::IsTextureBound(Ref<Texture> texture) const
	{
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (m_Textures[i] == texture)
				return true;
		}

		return false;
	}

	void VulkanShaderBindings::FlushTextureSlots()
	{
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			m_Textures[i] = m_FallbackTexture;
		}
		m_IsDirty = true;
	}

	void VulkanShaderBindings::SetSampler(const Ref<Texture>& texture, uint32_t slot)
	{
		if (slot >= m_Textures.size())
		{
			HYRO_LOG_CORE_ERROR("Tried to set a texture at slot {} but the shader only has {} slots. This may indicate a bug.", slot, m_Textures.size());
			return;
		}
		if (slot == 0)
		{
			HYRO_LOG_CORE_ERROR("Tried to set a texture at slot 0 but this slot is reserved for the fallback texture. This may indicate a bug.");
			return;
		}

		m_Textures[slot] = texture;

		m_IsDirty = true;
	}

	void VulkanShaderBindings::SetSamplerCube(const Ref<Cubemap>& cubemap)
	{
		if (!m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Tried to set a cubemap but the shader does not have a cubemap sampler. This may indicate a bug.");
			return;
		}

		m_Cubemap = cubemap;
		m_IsCubemapSet = true;
		m_IsDirty = true;
	}

	void VulkanShaderBindings::SetPushConstantBlock(const PushConstantBlock& block)
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

	void VulkanShaderBindings::Bind()
	{
		HYRO_LOG_CORE_ERROR("Tried to bind Material without a CommandBuffer. This may Indicate a Bug.");
	}

	void VulkanShaderBindings::Bind(void* commandBuffer)
	{
		if (!m_IsCubemapSet && m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Tried to bind Material without a Cubemap but the shader has a cubemap sampler. This may indicate a bug.");
		}

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

	void VulkanShaderBindings::UpdateDescriptorSets()
	{
		//Definitely needs to be reafactored but fine for now
		uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();

		std::vector<VkWriteDescriptorSet> writes(
			m_ReflectionData.Descriptors.size(),
			VkWriteDescriptorSet{}
		);


		std::vector<VkDescriptorImageInfo> imageInfos(m_Textures.size(), {});
		std::vector<VkDescriptorBufferInfo> bufferInfos(m_UniformBuffersByBinding.size(), {});
		VkDescriptorImageInfo cubemapInfo{};

		//Descriptor sets for each frame in flight
		//TODO: Updates Descriptors sets of all frames in flight, but this is not optimal. Should only update the current frame in flight
		for (uint32_t frameIndex = 0; frameIndex < maxFramesInFlight; frameIndex++)
		{
			size_t bufferIndex = 0;
			//Descriptor write for each descriptor/uniform
			for (uint32_t descriptorIndex = 0; descriptorIndex < m_ReflectionData.Descriptors.size(); ++descriptorIndex) {
				auto& descriptor = m_ReflectionData.Descriptors[descriptorIndex];

				writes[descriptorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writes[descriptorIndex].dstSet = m_DescriptorSets[frameIndex];
				writes[descriptorIndex].dstBinding = descriptor.Binding;
				writes[descriptorIndex].dstArrayElement = 0;
				writes[descriptorIndex].descriptorType = VulkanShader::HyroDescriptorTypeToVulkanType(descriptor.Type);
				writes[descriptorIndex].descriptorCount = descriptor.Count;
				if (descriptor.Type == DescriptorType::UniformBuffer)
				{
					VulkanUniformBuffer* vulkanUBO = static_cast<VulkanUniformBuffer*>(m_UniformBuffersByBinding.at(descriptor.Binding).get());

					VkDescriptorBufferInfo bufferInfo{};
					bufferInfos[bufferIndex].buffer = vulkanUBO->GetBufferAtIndex(frameIndex);
					bufferInfos[bufferIndex].offset = 0; //Offset is only requiered when ubo data is in the same buffer
					bufferInfos[bufferIndex].range = vulkanUBO->GetSize();

					writes[descriptorIndex].pBufferInfo = &bufferInfos[bufferIndex];
					++bufferIndex;
				}
				else if (descriptor.Type == DescriptorType::Sampler) {
					for (size_t imageIndex = 0; imageIndex < imageInfos.size(); imageIndex++)
					{
						VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(m_Textures[imageIndex].get());

						imageInfos[imageIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfos[imageIndex].imageView = vulkanTexture->GetVkImageView();
						imageInfos[imageIndex].sampler = vulkanTexture->GetVkSampler();

					}
					writes[descriptorIndex].pImageInfo = imageInfos.data();
				}						
				else  if (descriptor.Type == DescriptorType::SamplerCube) {
					VulkanCubemap* vulkanCubemap = static_cast<VulkanCubemap*>(m_Cubemap.get());

					cubemapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					cubemapInfo.imageView = vulkanCubemap->GetVkImageView();
					cubemapInfo.sampler = vulkanCubemap->GetVkSampler();

					writes[descriptorIndex].pImageInfo = &cubemapInfo;
				}
			}
			vkUpdateDescriptorSets(VulkanDevice::GetVkDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
		}

		m_IsDirty = false;
	}

}
