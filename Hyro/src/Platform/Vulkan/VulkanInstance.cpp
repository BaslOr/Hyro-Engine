#include "pch.h"
#include "Platform/Vulkan/VulkanInstance.h"

namespace Hyro {

	VulkanInstance::VulkanInstance()
	{
		CreateInstance();
		if (g_CurrentBuildConfig == BuildConfig::Debug)
			CreateDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		if (g_CurrentBuildConfig == BuildConfig::Debug)
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, g_VulkanAllocationCallback);	

		vkDestroyInstance(m_Instance, g_VulkanAllocationCallback);
	}

	void VulkanInstance::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_4;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pApplicationName = "APP";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "HYRO";

		auto layers = VulkanGetValidationLayers();
		auto extensions = GetAvailableInstanceExtensionNames();

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo{};
		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		if (g_CurrentBuildConfig == BuildConfig::Debug) {
			PopulateDebugMessengerCreateInfo(debugMessengerInfo);
			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

			instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
			instanceInfo.ppEnabledLayerNames = layers.data();
		}
		else {
			instanceInfo.enabledLayerCount = 0;
		}
		instanceInfo.pApplicationInfo = &appInfo;

		VkCheck(vkCreateInstance(&instanceInfo, g_VulkanAllocationCallback, &m_Instance));
	}

	void VulkanInstance::CreateDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
		PopulateDebugMessengerCreateInfo(messengerInfo);

		VkCheck(CreateDebugUtilsMessengerEXT(m_Instance, &messengerInfo, g_VulkanAllocationCallback, &m_DebugMessenger));
	}

	const std::vector<const char*> VulkanInstance::GetAvailableInstanceExtensionNames()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		if (extensionCount > 0)
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

		std::unordered_set<std::string> availableSet;

		for (const auto& ext : availableExtensions) {
			availableSet.insert(ext.extensionName);
		}

		const auto requiredExtensions = GetRequiredInstanceExtensions();

		for (const auto& extension : requiredExtensions) {
			if (availableSet.find(extension) == availableSet.end()) {
				HYRO_LOG_CORE_ERROR("Could not find the following Vulkan extension: {}", extension);
			}
		}

		return requiredExtensions;
	}

	void VulkanInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& messengerInfo)
	{
		messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messengerInfo.pfnUserCallback = DebugCallback;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			HYRO_LOG_CORE_ERROR("Validation Layer: {}", pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			HYRO_LOG_CORE_WARN("Validation Layer: {}", pCallbackData->pMessage);
		}
		else {
			HYRO_LOG_CORE_INFO("Validation Layer: {}", pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

}