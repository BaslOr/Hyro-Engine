#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Hyro/Core/Memory.h"
#include <cstdint>


namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI(const GraphicsPipelineSettings& settings);
		~VulkanAPI();

		void BeginScene() override;
		void EndScene() override;

		void Submit(Ref<VertexArray> vertexArray, Ref<UniformBuffer> uniformBuffer, Ref<Shader> shader, uint32_t count) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		void CreateSyncObjects();

	private:
		static inline VulkanAPI* s_Instance = nullptr;

		uint32_t m_ImageIndex = 0;

		Scope<VulkanGraphicsPipeline> m_Pipeline;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;

		std::vector<VkCommandBuffer> m_CommandBuffers;

		glm::vec4 m_ClearColor;
	};

}