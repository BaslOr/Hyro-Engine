#pragma once
#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/RenderPrimitives.h"
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/UniformBuffer.h"


namespace Hyro {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		static inline void BeginRenderPass() { RenderCommand::BeginRenderPass(); }
		static inline void EndRenderPass() { RenderCommand::EndRenderPass(); }

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

	private:
		friend class Renderer2D;
		friend class Renderer3D;

		static inline Ref<UniformBuffer> GetTransfromUniformBuffer() { return m_TransformUniformBuffer; }

	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::Vulkan;

		//Shared UBOs
		static inline Ref<UniformBuffer> m_TransformUniformBuffer;
	};

}