#pragma once
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/RenderingObjects/VertexArray.h"
#include "Hyro/Renderer/Material.h"
#include "Hyro/Renderer/Cubemap.h"

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

		virtual void BeginRenderPass() = 0;
		virtual void EndRenderPass() = 0;

		virtual void Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count) = 0;
		virtual void SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap) = 0;

		virtual void SetClearColor(const glm::vec4&	color) = 0;
	};

}