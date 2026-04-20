#include "pch.h"
#include "Platform/Vulkan/VulkanSurface.h"

#include "Hyro/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hyro {

	VulkanSurface::VulkanSurface(const Ref<VulkanInstance> instance)
		: m_Instance(instance)
	{
		auto window = Application::Get().GetWindow();
		glfwCreateWindowSurface(m_Instance->GetVkInstance(), (GLFWwindow*)window->GetNative(), g_VulkanAllocationCallback, &m_Surface);
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_Instance->GetVkInstance(), m_Surface, g_VulkanAllocationCallback);
	}

}