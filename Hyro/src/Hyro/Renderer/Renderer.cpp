#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Renderer/Renderer3D.h"
#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/Utils/MeshFactory.h"

#include <glm/gtc/type_ptr.hpp>

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
		vertexShaderPath = "Assets/Shaders/Cubemap.vert";
		fragmentShaderPath = "Assets/Shaders/Cubemap.frag";
		AssetManager::LoadShader("Cubemap", vertexShaderPath, fragmentShaderPath);

		//Inti Cubemap
		auto cubeVertices = MeshFactory::GetCubePositions();
		auto cubeIndices = MeshFactory::GetCubeIndices();
		auto cubemapShader = AssetManager::GetShader("Cubemap");
		m_CubemapVAO = VertexArray::Create();
		m_CubemapVBO = VertexBuffer::Create(cubemapShader->GetVertexLayout(), static_cast<uint32_t>(cubeVertices.size()));
		m_CubemapVBO->SetData(cubeVertices.data(), cubeVertices.size() * sizeof(glm::vec3));
		m_CubemapIBO = IndexBuffer::Create(static_cast<uint32_t>(cubeIndices.size()) * sizeof(uint32_t));
		m_CubemapIBO->SetData(cubeIndices);

		m_CubemapVAO->AddVertexBuffer(m_CubemapVBO);
		m_CubemapVAO->SetIndexBuffer(m_CubemapIBO);
		
		m_CubemapMaterial = Material::Create(cubemapShader);
		m_Cubemap = Cubemap::Create("Assets/Textures/Cubemap.hdr");



		if (m_GraphicsAPIType == GraphicsAPIType::OpenGL) {
			m_OpenGLTransformUniformBuffer = UniformBuffer::Create();
		}
		else if (m_GraphicsAPIType == GraphicsAPIType::Vulkan) {
			m_Vulkan2DTransformUniformBuffer = UniformBuffer::Create();
			m_Vulkan3DTransformUniformBuffer = UniformBuffer::Create();
		}


		//Init Subsystems/-components
		RenderCommand::Init(m_GraphicsAPIType);
		Renderer2D::Init();
		Renderer3D::Init();

		HYRO_LOG_CORE_TRACE("Initialized Renderer");
	}

	void SceneRenderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();

		HYRO_LOG_CORE_TRACE("Destroyed Renderer");
	}

	void SceneRenderer::BeginScene(const glm::mat4& mvp)
	{
		RenderCommand::BeginRenderPass();

		//Render Cubemap
		PushConstantBlock transfroms{};
		Uniform uniform("u_Model", DescriptorType::Matrix, (void*)glm::value_ptr(mvp));
		transfroms.Push(uniform);
		m_CubemapMaterial->SetPushConstantBlock(transfroms);
		RenderCommand::SubmitCubemap(m_CubemapVAO, m_CubemapMaterial, m_Cubemap);
	}

	void SceneRenderer::EndScene()
	{
		RenderCommand::EndRenderPass();
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