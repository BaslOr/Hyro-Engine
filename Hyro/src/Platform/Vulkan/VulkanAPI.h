#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"

namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI(const GraphicsPipelineSettings& settings);

		void DrawIndexed(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

	private:
		Ref<VulkanInstance> m_Instance; //Sets up Debug Messenger as well
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanGraphicsPipeline> m_GraphicsPipeline;
	};

}