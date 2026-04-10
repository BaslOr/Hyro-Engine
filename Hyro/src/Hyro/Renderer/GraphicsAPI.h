#pragma once
#include <glm/glm.hpp>

namespace Hyro {

	//Sending Commands to GPU
	class GraphicsAPI {
	public:
		virtual void DrawIndexed(uint32_t count) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4&	color) = 0;

		virtual void SetBlendFunction(bool enabled) = 0;
		virtual void SetSampleCount(uint32_t count) = 0;
	};

}