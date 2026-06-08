#include "pch.h"
#include "Platform/OpenGL/OpenGLBackend.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Hyro/Core/Core.h"

namespace Hyro {

	OpenGLBackend::OpenGLBackend(void* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLBackend::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			HYRO_LOG_CORE_FATAL("Failed to initialize GLAD");
		}
	}

	void OpenGLBackend::SwapBuffers() const
	{
		glfwSwapBuffers((GLFWwindow*)m_WindowHandle);
	}

	void OpenGLBackend::ResizeViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

}