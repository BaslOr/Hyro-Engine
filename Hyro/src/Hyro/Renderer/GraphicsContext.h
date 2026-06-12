#pragma once
#include "Hyro/Renderer/Renderer.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	//Only handles connection to GPU/Window, not rendering itself
	//Commands for rendering should be in GraphicsAPI class
	class GraphicsContext {
	public:
		static Scope<GraphicsContext> Create(GraphicsAPIType type, void* windowHandle);

		virtual void Init() = 0;
		virtual void SwapBuffers() const = 0;
		virtual void ResizeViewport(uint32_t width, uint32_t height) = 0;
	};

}
