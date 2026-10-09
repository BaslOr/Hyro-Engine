#pragma once
#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"
#include "Hyro/Renderer/ShaderBindings.h"
#include "Hyro/Renderer/RenderPrimitives.h"

#include <array>


namespace Hyro {

	struct Renderer3DData {
		Ref<Shader> Shader;
		Ref<ShaderBindings> ShaderBinding;
		Ref<UniformBuffer> TransformBuffer;
		Ref<UniformBuffer> MaterialBuffer;
	};


	class Renderer3D {
	public:
		static void DrawMesh(const Ref<Mesh>& mesh, const Ref<Material>& surface, const glm::mat4& transform);

	private:
		static void BeginScene(const glm::mat4& mvp);
		static void EndScene();

		static void Init();
		static void Shutdown();		

	private:

		inline static Renderer3DData m_Data;

		friend class Scene;
		friend class SceneRenderer;
	};

}
