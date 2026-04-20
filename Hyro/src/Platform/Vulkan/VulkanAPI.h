#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/Vulkan/VulkanInstance.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

namespace Hyro {

	class VulkanAPI : public GraphicsAPI {
	public:
		VulkanAPI();

		void DrawIndexed(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

	private:
		Ref<VulkanInstance> m_Instance; //Sets up Debug Messenger
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
	};

}