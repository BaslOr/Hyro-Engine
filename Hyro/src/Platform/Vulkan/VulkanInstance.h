#pragma once
#include "Platform/Vulkan/VulkanBase.h"


namespace Hyro {

	class VulkanInstance {
	public:
		VulkanInstance();
		~VulkanInstance();

		inline VkInstance GetVkInstance() const { return m_Instance; }
	private:
		void CreateInstance();
		void CreateDebugMessenger();

		const std::vector<const char*> GetAvailableInstanceExtensionNames();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& messengerInfo);
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}