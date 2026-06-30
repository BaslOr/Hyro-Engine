#pragma once
#include "Hyro/Renderer/Shader.h"

#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"


namespace Hyro {

	class VulkanShader : public Shader {
	public: 
		VulkanShader(const GraphicsPipelineSettings& settings);
		~VulkanShader() = default;

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

	private:
		Ref<VulkanGraphicsPipeline> m_Pipeline;
		Ref<UniformBuffer> m_UniformBuffer;

	};
}