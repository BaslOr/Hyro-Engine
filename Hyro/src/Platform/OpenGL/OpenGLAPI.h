#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

namespace Hyro {
	
	class OpenGLAPI : public GraphicsAPI {
	public:
		OpenGLAPI();

		void BeginRenderPass() override;
		void EndRenderPass() override;

		void Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count) override;
		void SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		void SetupDebugCallback();

	};

}