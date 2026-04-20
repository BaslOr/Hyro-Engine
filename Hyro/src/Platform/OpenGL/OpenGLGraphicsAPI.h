#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

namespace Hyro {
	
	class OpenGLGraphicsAPI : public GraphicsAPI {
	public:
		OpenGLGraphicsAPI();

		void DrawIndexed(uint32_t count) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

	private:
		void SetupDebugCallback();

	};

}