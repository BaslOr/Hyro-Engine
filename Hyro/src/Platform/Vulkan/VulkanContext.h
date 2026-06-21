#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

#include "Platform/Vulkan/VulkanSurface.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

namespace Hyro {

	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(void* windowHandle);
		~VulkanContext();

		VulkanContext(const VulkanContext&) = delete;
		VulkanContext& operator=(const VulkanContext&) = delete;
		VulkanContext(VulkanContext&&) = delete;
		VulkanContext& operator=(VulkanContext&&) = delete;

		void Init() override {}
		void SwapBuffers() const override {}
		void ResizeViewport(uint32_t width, uint32_t height) override;

		VkFormat GetSwapchainImageFormat() const { return m_Swapchain->GetImageFormat(); } //Later move to modern pipeline thing, look up on claude ;)
		VkRenderPass GetRenderPass() const { return m_Swapchain->GetRenderPass(); }

		inline uint16_t GetMaxFramesInFlight() const { return m_MaxFramesInFlight; }
		inline uint16_t GetCurrentFrameIndex() const { return m_CurrentFrame; }
		inline void IncreaseImageIndex() { m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight; }


		//Temporary until we have a better way to pass the Vulkan objects to the ImGui Vulkan backend
		inline VkInstance GetInstance() const { return m_Instance->GetVkInstance(); }
		inline VkSurfaceKHR GetSurface() const { return m_Surface->GetVkSurface(); }
		inline VkSwapchainKHR GetSwapchain() const { return m_Swapchain->GetVkSwapchain(); }
		inline VkFramebuffer GetVkFramebuffer(size_t index) const { return m_Swapchain->GetVkFramebuffers()[index]; }
		inline uint32_t GetMinImageCount() const { return m_Swapchain->GetMinImageCount();; }
		inline uint32_t GetImageCount() const { return static_cast<uint32_t>(m_Swapchain->GetImageViews().size()); }
		inline VkExtent2D GetSwapchainExtent() const { return m_Swapchain->GetExtent(); }

		inline static VulkanContext& Get() { return *s_Instance; }

	private:
		static inline VulkanContext* s_Instance = nullptr;

	private:
		Ref<VulkanInstance> m_Instance; //Sets up Debug Messenger as well
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanSwapchain> m_Swapchain;


		size_t m_MaxFramesInFlight = 2;
		size_t m_CurrentFrame = 0;

	};

}