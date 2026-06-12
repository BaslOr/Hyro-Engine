#include "pch.h"
#include "Platform/Vulkan/VulkanSurface.h"

#include "Hyro/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hyro {

	VulkanSurface::VulkanSurface(const Ref<VulkanInstance> instance, void* windowHandle)
		: m_Instance(instance)
	{
		glfwCreateWindowSurface(m_Instance->GetVkInstance(), (GLFWwindow*)windowHandle, g_VulkanAllocationCallback, &m_Surface);
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_Instance->GetVkInstance(), m_Surface, g_VulkanAllocationCallback);
	}

}