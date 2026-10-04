#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/RenderingObjects/VertexArray.h"
#include "Hyro/Renderer/ShaderBindings.h"
#include <glm/fwd.hpp>

namespace Hyro {

	class RenderCommand {
	public:
		static void Init(GraphicsAPIType type);

		static void BeginRenderPass();
		static void EndRenderPass();

		static void Submit(Ref<VertexArray> vertexArray, Ref<ShaderBindings> material, uint32_t count);
		static void SubmitCubemap(Ref<VertexArray> vertexArray, Ref<ShaderBindings> material, Ref<Cubemap> cubemap);

		static void SetClearColor(const glm::vec4& color);


	private:
		static inline Scope<GraphicsAPI> m_API;
	};

}
