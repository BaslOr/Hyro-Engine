#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Project/AssetManager.h"

namespace Hyro {

	void Renderer::Init()
	{
		//Set Default Vertex Layout

		//Some kind of API description should be passed here
		//To determine blend func, sample count, ...
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
		if (Renderer::GetAPI() == GraphicsAPIType::Vulkan) {
			vertexShaderPath = "Assets/Shaders/vert.spv";
			fragmentShaderPath = "Assets/Shaders/frag.spv";
		}
		else if (Renderer::GetAPI() == GraphicsAPIType::OpenGL)
		{
			vertexShaderPath = "Assets/Shaders/vertex.glsl";
			fragmentShaderPath = "Assets/Shaders/fragment.glsl";
		}
		AssetManager::LoadShader("Default2D", vertexShaderPath, fragmentShaderPath);

		RenderCommand::Init(m_GraphicsAPIType);
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
	}

}