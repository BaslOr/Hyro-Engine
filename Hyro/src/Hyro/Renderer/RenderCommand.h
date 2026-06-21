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
		static void Init(GraphicsAPIType type);

		static void BeginScene();
		static void EndScene();

		static void Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count);

		static void SetClearColor(const glm::vec4& color);


	private:
		static inline Scope <GraphicsAPI> m_API;
	};

}
