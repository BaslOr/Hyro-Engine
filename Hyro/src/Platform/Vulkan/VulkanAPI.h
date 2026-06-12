#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanCommandPool.h"


namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI(const GraphicsPipelineSettings& settings);

		void DrawIndexed(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

	private:
		Scope<VulkanGraphicsPipeline> m_Pipeline;

		VkCommandBuffer m_CommandBuffer;
	};

}