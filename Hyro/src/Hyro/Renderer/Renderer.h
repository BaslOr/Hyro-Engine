#pragma once
#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Cubemap.h"


namespace Hyro {

	class SceneRenderer {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& mvp);
		static void EndScene();

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

		static Ref<UniformBuffer> GetRenderer2DTransformUnifromBuffer();
		static Ref<UniformBuffer> GetRenderer3DTransformUnifromBuffer();

	private:
		friend class Renderer2D;
		friend class Renderer3D;


	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::OpenGL;

		static inline Ref<Cubemap> m_Cubemap;
		static inline Ref<Material> m_CubemapMaterial;
		static inline Ref<VertexArray> m_CubemapVAO;
		static inline Ref<VertexBuffer> m_CubemapVBO;
		static inline Ref<IndexBuffer> m_CubemapIBO;
		
		static inline Ref<UniformBuffer> m_OpenGLTransformUniformBuffer;
		static inline Ref<UniformBuffer> m_Vulkan2DTransformUniformBuffer;
		static inline Ref<UniformBuffer> m_Vulkan3DTransformUniformBuffer;

	};

}