#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

#include "Hyro/Renderer/RenderingObjects/VertexBuffer.h"
#include "Hyro/Renderer/RenderingObjects/IndexBuffer.h"
#include "Hyro/Renderer/Shader.h"



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
