#pragma once
#include "Hyro/Renderer/GraphicsBackend.h"


namespace Hyro {

	class OpenGLBackend : public GraphicsBackend {
	public:
		OpenGLBackend(void* windowHandle);

		void Init() override;
		void SwapBuffers() const override;
		void ResizeViewport(uint32_t width, uint32_t height) override;

	private:
		void* m_WindowHandle;
	};

}
