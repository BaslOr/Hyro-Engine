#pragma once
#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/RenderPrimitives.h"
#include "Hyro/Renderer/RenderCommand.h"


namespace Hyro {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		static void DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform);

		static inline void BeginRenderPass() { RenderCommand::BeginRenderPass(); }
		static inline void EndRenderPass() { RenderCommand::EndRenderPass(); }

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::Vulkan;
	};

}