#include "pch.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

#include <glad/glad.h>

namespace Hyro {



	void OpenGLGraphicsAPI::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLGraphicsAPI::SetBlendFunction(bool enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLGraphicsAPI::SetSampleCount(uint32_t count)
	{
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);
	}

	void OpenGLGraphicsAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

}