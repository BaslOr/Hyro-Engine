#pragma once

namespace Hyro {

	//Only handles connection to GPU/Window, not rendering itself
	//Commands for rendering should be in GraphicsAPI class
	class GraphicsContext {
	public:

		virtual void Init() = 0;
		virtual void SwapBuffers() const = 0;
		virtual void ResizeViewport(uint32_t width, uint32_t height) = 0;
	};

}
