#pragma once
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/GraphicsPipeline.h"
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/VertexArray.h"
#include "Hyro/Renderer/UniformBuffer.h"

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

		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;

		virtual void Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count) = 0;

		virtual void SetClearColor(const glm::vec4&	color) = 0;
	};

}