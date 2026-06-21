#include "pch.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Hyro {

	VulkanShader::VulkanShader(const GraphicsPipelineSettings& settings)
	{
		m_Pipeline = CreateRef<VulkanGraphicsPipeline>(settings);
		m_UniformBuffer = UniformBuffer::Create();
	}

	void VulkanShader::Bind() const
	{
		HYRO_LOG_CORE_WARN("Tried to bind OpenGL Shader on Vulkan side. This may indicate a bug.");
	}

	void VulkanShader::Bind(void* commandBuffer) const
	{
		m_UniformBuffer->Bind(commandBuffer, m_Pipeline->GetVkPipelineLayout());

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

	void VulkanShader::setUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		UniformBufferData data{};
		data.Projection = value;
		m_UniformBuffer->SetData(data);
	}

}
