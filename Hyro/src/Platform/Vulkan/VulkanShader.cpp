#include "pch.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"

namespace Hyro {

	VulkanShader::VulkanShader(const GraphicsPipelineSettings& settings)
	{
		m_Pipeline = CreateRef<VulkanGraphicsPipeline>(settings);

		uint16_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		m_DescriptorSets.resize(maxFramesInFlight);
		m_DescriptorSets = VulkanDescriptorPool::AllocateDescriptorSets(VulkanUniformBuffer::GetDescriptorSetLayout(), maxFramesInFlight);


	}

	void VulkanShader::Bind() const
	{
		HYRO_LOG_CORE_WARN("Tried to bind OpenGL Shader on Vulkan side. This may indicate a bug.");
	}

	void VulkanShader::Bind(void* commandBuffer) const
	{
		uint32_t currentFrame = VulkanContext::Get().GetCurrentFrameIndex();

		vkCmdBindDescriptorSets((VkCommandBuffer)commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_Pipeline->GetVkPipelineLayout(), 0, 1, &m_DescriptorSets[currentFrame], 0, nullptr);
		vkCmdBindPipeline((VkCommandBuffer)commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetVkPipeline());

		VulkanContext& context = VulkanContext::Get();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(context.GetSwapchainExtent().width);
		viewport.height = static_cast<float>(context.GetSwapchainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport((VkCommandBuffer)commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = context.GetSwapchainExtent();
		vkCmdSetScissor((VkCommandBuffer)commandBuffer, 0, 1, &scissor);
	}

	void VulkanShader::BindUBO(const Ref<UniformBuffer>& ubo)
	{
		uint16_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();
		auto vulkanUBO = static_cast<VulkanUniformBuffer*>(ubo.get());

		for (size_t i = 0; i < maxFramesInFlight; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = vulkanUBO->GetBufferAtIndex(i);
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferData);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(VulkanDevice::GetVkDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}

}
