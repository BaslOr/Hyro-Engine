#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/VertexBuffer.h"
#include "Hyro/Renderer/IndexBuffer.h"
#include "Hyro/Renderer/VertexArray.h"
#include "Hyro/Renderer/Shader.h"
#include <glm/fwd.hpp>

namespace Hyro {

	class RenderCommand {
	public:
		static void Init(GraphicsAPIType type, const GraphicsPipelineSettings& pipelineSettings);

		static void DrawIndexed(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count);

		static void Clear();
		static void SetClearColor(const glm::vec4& color);

		static void Submit();
		static void Submit(Ref<VertexArray> vertexArray);
		static void Submit(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer);


	private:
		static inline Scope <GraphicsAPI> m_API;
	};

}
