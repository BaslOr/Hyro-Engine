#pragma once
#include <cstdint>

namespace Hyro {

	enum class GraphicsAPI {
		None = 0, OpenGL, Vulkan
	};

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() const = 0;
		virtual void ResizeViewport(uint32_t width, uint32_t height) = 0;

	};

}
