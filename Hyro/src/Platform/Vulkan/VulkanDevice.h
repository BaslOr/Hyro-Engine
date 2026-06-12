#pragma once
#include "Platform/Vulkan/VulkanBase.h"

#include "VulkanInstance.h"
#include "VulkanSurface.h"

#include <optional>

namespace Hyro {

	struct QueueFamilyIndices {
		std::optional<uint32_t> GraphcisQueueFamily;
		std::optional<uint32_t> PresentationQueueFamily;
		std::optional<uint32_t> TransferQueueFamily;

		bool IsComplete() const {
			return GraphcisQueueFamily.has_value() && PresentationQueueFamily.has_value() && TransferQueueFamily.has_value();
		}
	};

	class VulkanDevice {
	public:
		static void Init(const Ref<VulkanInstance> instance, const Ref<VulkanSurface> surface);
		static void Destroy();

		inline static VkDevice GetVkDevice() { return m_Device; }
		inline static VkPhysicalDevice GetVkPhysicalDevice() { return m_PhysicalDevice; }
			   
		inline static QueueFamilyIndices GetQueueFamilyIndices() { return m_QueueFamilyIndices; }
			   
		inline static VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		inline static VkQueue GetPresentationQueue() { return m_PresentationQueue; }
		inline static VkQueue GetTransferQueue() { return m_TransferQueue; }

	private:
		static void PickPhysicalDevice(VkInstance instance);
		static void CreateDeviceAndQueues();

		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static int GetPhysicalDeviceScore(VkPhysicalDevice device);

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	private:
		static inline VkPhysicalDevice m_PhysicalDevice;
		static inline VkDevice m_Device;
		static inline VkSurfaceKHR m_Surface;
			    
		static inline VkQueue m_GraphicsQueue;
		static inline VkQueue m_PresentationQueue;
		static inline VkQueue m_TransferQueue;
			    
		static inline QueueFamilyIndices m_QueueFamilyIndices;
	};

}