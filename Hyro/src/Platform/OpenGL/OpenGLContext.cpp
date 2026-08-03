#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Hyro/Core/Core.h"

namespace Hyro {

	OpenGLContext::OpenGLContext(void* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			HYRO_LOG_CORE_FATAL("Failed to initialize GLAD");
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers((GLFWwindow*)m_WindowHandle);
	}

	void OpenGLContext::ResizeViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

}