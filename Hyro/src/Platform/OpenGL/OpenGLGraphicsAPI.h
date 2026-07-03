#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

namespace Hyro {
	
	class OpenGLGraphicsAPI : public GraphicsAPI {
	public:
		OpenGLGraphicsAPI();

		void BeginRenderPass() override;
		void EndRenderPass() override {}

		void Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		void SetupDebugCallback();

	};

}