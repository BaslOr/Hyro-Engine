#include "pch.h"
#include "Platform/Vulkan/VulkanDevice.h"

#include <Hyro/Core/Core.h>

namespace Hyro {


	VulkanDevice::VulkanDevice(const Ref<VulkanInstance> instance, const Ref<VulkanSurface> surface)
		: m_Surface(surface->GetVkSurface())
	{
		PickPhysicalDevice(instance->GetVkInstance());
		CreateDeviceAndQueues();

		m_QueueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

		HYRO_LOG_CORE_TRACE("Created Vulkan Device");
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_Device, g_VulkanAllocationCallback);
	}


	void VulkanDevice::PickPhysicalDevice(VkInstance instance)
	{
		uint32_t count = 0;
		vkEnumeratePhysicalDevices(instance, &count, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(count);
		vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());

		VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
		int bestScore = -1;

		for (auto device : physicalDevices) {
			if (IsDeviceSuitable(device)) {
				int score = GetPhysicalDeviceScore(device);
				if (score > bestScore) {
					bestScore = score;
					bestDevice = device;
				}
			}
		}

		if (bestDevice == VK_NULL_HANDLE) {
			HYRO_LOG_CORE_ERROR("Failed to find suitable Vulkan Device");
		}

		m_PhysicalDevice = bestDevice;
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);
		HYRO_LOG_CORE_INFO("Selected: {} GPU", properties.deviceName);
	}

	void VulkanDevice::CreateDeviceAndQueues()
	{
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);
		std::set<uint32_t> uniqueQueueFamilies = {
			queueFamilyIndices.GraphcisQueueFamily.value(),
			queueFamilyIndices.PresentationQueueFamily.value(),
			queueFamilyIndices.TransferQueueFamily.value()
		};
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		float queuePriority = 1.0f;
		for (auto queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		const auto layers = VulkanGetValidationLayers();
		const auto extensions = VulkanGetRequiredDeviceExtensions();

		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		deviceInfo.ppEnabledLayerNames = layers.data();
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		deviceInfo.ppEnabledExtensionNames = extensions.data();
		deviceInfo.pEnabledFeatures = nullptr;

		VkCheck(vkCreateDevice(m_PhysicalDevice, &deviceInfo, g_VulkanAllocationCallback, &m_Device));

		vkGetDeviceQueue(m_Device, queueFamilyIndices.GraphcisQueueFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, queueFamilyIndices.PresentationQueueFamily.value(), 0, &m_PresentationQueue);
		vkGetDeviceQueue(m_Device, queueFamilyIndices.TransferQueueFamily.value(), 0, &m_TransferQueue);
	}

   bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
   {
	   QueueFamilyIndices indices = FindQueueFamilies(device);

	   return indices.IsComplete();
   }

   int VulkanDevice::GetPhysicalDeviceScore(VkPhysicalDevice device)
   {
	   VkPhysicalDeviceProperties properties;
	   vkGetPhysicalDeviceProperties(device, &properties);

	   int score = 0;

	   // Discrete GPUs have a significant performance advantage
	   if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		   score += 1000;
	   }

	   return score;
   }

   QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
   {
	   QueueFamilyIndices indices;

	   uint32_t queueFamilyCount = 0;
	   vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	   std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	   vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	   for (uint32_t i = 0; i < queueFamilyCount; i++) {
		   if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			   indices.GraphcisQueueFamily = i;
		   }

		   VkBool32 presentSupport = false;
		   vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
		   if (presentSupport) {
			   indices.PresentationQueueFamily = i;
		   }

		   if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			   indices.TransferQueueFamily = i;
		   }

		   if (indices.GraphcisQueueFamily.has_value() && indices.PresentationQueueFamily.has_value() && indices.TransferQueueFamily.has_value()) {
			   break;
		   }
	   }	

	   return indices;
   }

}