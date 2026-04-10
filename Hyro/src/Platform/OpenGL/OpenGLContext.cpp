#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Hyro/Core/Core.h"

namespace Hyro {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			HYRO_LOG_CORE_FATAL("Failed to initialize GLAD");
		}
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::ResizeViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

}