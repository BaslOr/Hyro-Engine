#pragma once
#include "Hyro/Renderer/Shader.h"

#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"


namespace Hyro {

	class VulkanShader : public Shader {
	public: 
		VulkanShader(const GraphicsPipelineSettings& settings);
		~VulkanShader();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		VkPipelineLayout GetVkPipelineLayout() const { return m_Pipeline->GetVkPipelineLayout(); }
		VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return m_Pipeline->GetVkDescriptorSetLayout(); }

	private:
		Ref<VulkanGraphicsPipeline> m_Pipeline;
	};
}