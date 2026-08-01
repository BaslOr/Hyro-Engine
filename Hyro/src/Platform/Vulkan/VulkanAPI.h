#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Hyro/Core/Memory.h"
#include <Hyro/Renderer/Cubemap.h>


namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI();
		~VulkanAPI();

		void BeginRenderPass() override;
		void EndRenderPass() override;

		void Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count) override;
		void SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		void CreateSyncObjects();

	private:
		static inline VulkanAPI* s_Instance = nullptr;

		uint32_t m_ImageIndex = 0;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;

		std::vector<VkCommandBuffer> m_CommandBuffers;

		glm::vec4 m_ClearColor;
	};

}