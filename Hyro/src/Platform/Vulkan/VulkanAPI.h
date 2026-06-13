#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanCommandPool.h"


namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI(const GraphicsPipelineSettings& settings);
		~VulkanAPI();

		void Submit() override;
		void DrawIndexed(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

		inline VkCommandBuffer GetVkCommandBuffer() const { return m_CommandBuffer; }


		inline static VulkanAPI& Get() { return *s_Instance; }

	private:
		void CreateSyncObjects();

	private:
		static inline VulkanAPI* s_Instance = nullptr;

		Scope<VulkanGraphicsPipeline> m_Pipeline;

		size_t m_MaxFramesInFlight = 2;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;

		std::vector<VkCommandBuffer> m_CommandBuffer;
	};

}