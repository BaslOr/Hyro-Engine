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
		std::string vertexShaderPath = "Assets/Shaders/shader.vert";
		std::string fragmentShaderPath = "Assets/Shaders/shader.frag";
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