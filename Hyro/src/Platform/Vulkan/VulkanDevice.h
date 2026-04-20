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
		VulkanDevice(const Ref<VulkanInstance> instance, const Ref<VulkanSurface> surface);
		~VulkanDevice();

		inline VkDevice GetDevice() const { return m_Device; }
		inline VkPhysicalDevice GetVkPhysicalDevice() const { return m_PhysicalDevice; }

		inline QueueFamilyIndices GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		inline VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		inline VkQueue GetPresentationQueue() const { return m_PresentationQueue; }
		inline VkQueue GetTransferQueue() const { return m_TransferQueue; }

	private:
		void PickPhysicalDevice(VkInstance instance);
		void CreateDeviceAndQueues();

		bool IsDeviceSuitable(VkPhysicalDevice device);
		int GetPhysicalDeviceScore(VkPhysicalDevice device);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	private:
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;

		VkSurfaceKHR m_Surface;
		
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentationQueue;
		VkQueue m_TransferQueue;

		QueueFamilyIndices m_QueueFamilyIndices;
	};

}