#pragma once
#include <vulkan/vulkan.h>

#include "Hyro/Core/Core.h"

#include <GLFW/glfw3.h> // Temporary

namespace Hyro {

    constexpr VkAllocationCallbacks* g_VulkanAllocationCallback = nullptr;

    //Load required functions
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT messenger,
        const VkAllocationCallbacks* pAllocator)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func != NULL) {
            func(instance, messenger, pAllocator);
        }
    }

	static void  VkCheck(VkResult result) {
        switch (result)
        {
        case VK_SUCCESS:
            break;

        case VK_NOT_READY:
        case VK_TIMEOUT:
        case VK_EVENT_SET:
        case VK_EVENT_RESET:
        case VK_INCOMPLETE:
        case VK_SUBOPTIMAL_KHR:
        case VK_THREAD_IDLE_KHR:
        case VK_THREAD_DONE_KHR:
        case VK_OPERATION_DEFERRED_KHR:
        case VK_OPERATION_NOT_DEFERRED_KHR:
            // Kein harter Fehler, nur Status / Warnung
            HYRO_LOG_CORE_ERROR("Vulkan non-fatal status code");
            break;

        case VK_ERROR_OUT_OF_HOST_MEMORY:
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            // Kritischer Zustand: System ist am Limit
            HYRO_LOG_CORE_FATAL("Vulkan out of memory");
            break;

        case VK_ERROR_DEVICE_LOST:
            // GPU weg → praktisch nicht recoverable
            HYRO_LOG_CORE_FATAL("Vulkan device lost!");
            break;

        case VK_ERROR_INITIALIZATION_FAILED:
        case VK_ERROR_INCOMPATIBLE_DRIVER:
        case VK_ERROR_EXTENSION_NOT_PRESENT:
        case VK_ERROR_FEATURE_NOT_PRESENT:
           HYRO_LOG_CORE_FATAL("Vulkan initialization/configuration failure");
            break;

        case VK_ERROR_LAYER_NOT_PRESENT:
        case VK_ERROR_VALIDATION_FAILED:
            HYRO_LOG_CORE_ERROR("Vulkan validation/layer issue");
            break;

        case VK_ERROR_OUT_OF_DATE_KHR:
        case VK_ERROR_SURFACE_LOST_KHR:
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            HYRO_LOG_CORE_ERROR("Swapchain/surface issue (needs recreation likely)");
            break;

        case VK_ERROR_UNKNOWN:
        default:
            HYRO_LOG_CORE_FATAL("Unknown or unhandled Vulkan error");
            break;
        }
	}

    static const std::vector<const char*> VulkanGetValidationLayers() {
        std::vector<const char*> layers;
        layers.push_back("VK_LAYER_KHRONOS_validation");

        return layers;
    }

    static const std::vector<const char*> GetRequiredInstanceExtensions() {
        uint32_t count = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);//TODO: Move to Window Class

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

        extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);//Required for MacOS
        if (g_CurrentBuildConfig == BuildConfig::Debug)
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    static const std::vector<const char*> VulkanGetRequiredDeviceExtensions() {
        std::vector<const char*> extensions;
        
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        return extensions;
    }

}
