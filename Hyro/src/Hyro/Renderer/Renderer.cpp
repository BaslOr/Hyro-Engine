#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Renderer/Renderer3D.h"
#include "Hyro/Project/AssetManager.h"

namespace Hyro {

	void Renderer::Init()
	{
		//Set Default Vertex Layout

		//Some kind of API description should be passed here
		//To determine blend func, sample count, ...
		AssetManager::LoadTexture("Default", "Assets/Textures/Fallback.png");
		std::string vertexShaderPath = "Assets/Shaders/Shader2D.vert";
		std::string fragmentShaderPath = "Assets/Shaders/Shader2D.frag";
		AssetManager::LoadShader("Default2D", vertexShaderPath, fragmentShaderPath);
		vertexShaderPath = "Assets/Shaders/Shader3D.vert";
		fragmentShaderPath = "Assets/Shaders/Shader3D.frag";
		AssetManager::LoadShader("Default3D", vertexShaderPath, fragmentShaderPath);

		m_TransformUniformBuffer = UniformBuffer::Create();

		RenderCommand::Init(m_GraphicsAPIType);
		Renderer2D::Init();
		Renderer3D::Init();

		HYRO_LOG_CORE_TRACE("Initialized Renderer");
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();

		HYRO_LOG_CORE_TRACE("Destroyed Renderer");
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

}