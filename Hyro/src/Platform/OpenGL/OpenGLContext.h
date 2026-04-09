#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

#include <glad/glad.h>

struct GLFWwindow;

namespace Hyro {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() const override;
		virtual void ResizeViewport(uint32_t width, uint32_t height) override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}
