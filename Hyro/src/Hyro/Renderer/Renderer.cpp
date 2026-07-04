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
		std::string vertexShaderPath = "Assets/Shaders/Shader2D.vert";
		std::string fragmentShaderPath = "Assets/Shaders/Shader2D.frag";
		AssetManager::LoadShader("Default2D", vertexShaderPath, fragmentShaderPath);
		vertexShaderPath = "Assets/Shaders/Shader3D.vert";
		fragmentShaderPath = "Assets/Shaders/Shader3D.frag";
		AssetManager::LoadShader("Default3D", vertexShaderPath, fragmentShaderPath);

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

}