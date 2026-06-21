#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

namespace Hyro {
	
	class OpenGLGraphicsAPI : public GraphicsAPI {
	public:
		OpenGLGraphicsAPI();

		void BeginScene() override;
		void EndScene() override {}

		void Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		void SetupDebugCallback();

	};

}