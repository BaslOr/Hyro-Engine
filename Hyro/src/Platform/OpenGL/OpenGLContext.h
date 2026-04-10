#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

#include <glad/glad.h>

struct GLFWwindow;

namespace Hyro {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() const override;
		void ResizeViewport(uint32_t width, uint32_t height) override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}
