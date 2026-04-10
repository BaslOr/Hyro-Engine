#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

namespace Hyro {
	
	class OpenGLGraphicsAPI : public GraphicsAPI {
	public:
		void DrawIndexed(uint32_t count) override;
		void SetBlendFunction(bool enabled) override;
		void SetSampleCount(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

		void SetUpDebugCallback() override;
	};

}