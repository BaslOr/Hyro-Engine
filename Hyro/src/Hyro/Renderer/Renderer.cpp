#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Renderer/Renderer3D.h"
#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/Utils/ModelLoader.h"

namespace Hyro {

	void SceneRenderer::Init()
	{
		//Some kind of API description should be passed here
		//To determine blend func, sample count, ...
		AssetManager::LoadTexture("Default", "Assets/Textures/Fallback.png");
		std::string vertexShaderPath = "Assets/Shaders/Shader2D.vert";
		std::string fragmentShaderPath = "Assets/Shaders/Shader2D.frag";
		AssetManager::LoadShader("Default2D", vertexShaderPath, fragmentShaderPath);
		vertexShaderPath = "Assets/Shaders/Shader3D.vert";
		fragmentShaderPath = "Assets/Shaders/Shader3D.frag";
		AssetManager::LoadShader("Default3D", vertexShaderPath, fragmentShaderPath);

		if (m_GraphicsAPIType == GraphicsAPIType::OpenGL) {
			m_OpenGLTransformUniformBuffer = UniformBuffer::Create();
		}
		else if (m_GraphicsAPIType == GraphicsAPIType::Vulkan) {
			m_Vulkan2DTransformUniformBuffer = UniformBuffer::Create();
			m_Vulkan3DTransformUniformBuffer = UniformBuffer::Create();

		}

		RenderCommand::Init(m_GraphicsAPIType);
		Renderer2D::Init();
		Renderer3D::Init();

		ModelLoader loader();

		HYRO_LOG_CORE_TRACE("Initialized Renderer");
	}

	void SceneRenderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();

		HYRO_LOG_CORE_TRACE("Destroyed Renderer");
	}

	void SceneRenderer::BeginScene()
	{
	}

	void SceneRenderer::EndScene()
	{
	}

	//Find better Solution in future
	//The problem is that OpenGL requieres one UBO while Vulkan requiers two
	//Solve this problem when adding reflection to shader or sumn
	Ref<UniformBuffer> SceneRenderer::GetRenderer2DTransformUnifromBuffer()
	{
		if (m_GraphicsAPIType == GraphicsAPIType::OpenGL) {
			return m_OpenGLTransformUniformBuffer;
		}
		else if (m_GraphicsAPIType == GraphicsAPIType::Vulkan) {
			return m_Vulkan2DTransformUniformBuffer;
		}
	}

	Ref<UniformBuffer> SceneRenderer::GetRenderer3DTransformUnifromBuffer()
	{
		if (m_GraphicsAPIType == GraphicsAPIType::OpenGL) {
			return m_OpenGLTransformUniformBuffer;
		}
		else if (m_GraphicsAPIType == GraphicsAPIType::Vulkan) {
			return m_Vulkan3DTransformUniformBuffer;
		}
	}

}