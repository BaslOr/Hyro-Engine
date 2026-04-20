#pragma once
#include <glm/glm.hpp>

namespace Hyro {

	enum class GraphicsAPIType {
		None = 0,
		OpenGL = 1,
		Vulkan = 2
	};

	//Sending Commands to GPU
	class GraphicsAPI {
	public:
		static Scope<GraphicsAPI> Create(GraphicsAPIType api);

		virtual void DrawIndexed(uint32_t count) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4&	color) = 0;
	};

}