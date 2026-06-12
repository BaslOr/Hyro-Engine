#pragma once
#include "Hyro/Renderer/GraphicsContext.h"


namespace Hyro {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(void* windowHandle);

		void Init() override;
		void SwapBuffers() const override;
		void ResizeViewport(uint32_t width, uint32_t height) override;

	private:
		void* m_WindowHandle;
	};

}
