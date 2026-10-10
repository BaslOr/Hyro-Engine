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
		DepthInfo depthInfo{};
		AssetManager::LoadShader("Default2D", depthInfo, vertexShaderPath, fragmentShaderPath);
		vertexShaderPath = "Assets/Shaders/PBR.vert";
		fragmentShaderPath = "Assets/Shaders/PBR.frag";
		AssetManager::LoadShader("PBR", depthInfo, vertexShaderPath, fragmentShaderPath);
		vertexShaderPath = "Assets/Shaders/Cubemap.vert";
		fragmentShaderPath = "Assets/Shaders/Cubemap.frag";
		depthInfo.DepthWrite = false;
		depthInfo.DepthFunc = DepthInfo::CompareOp::LessEqual;
		AssetManager::LoadShader("Cubemap", depthInfo, vertexShaderPath, fragmentShaderPath);

		//Set Default Materials
		AssetManager::LoadMaterial("Default2D", AssetManager::GetShader("Default2D"));
		AssetManager::LoadMaterial("Default3D", AssetManager::GetShader("PBR"));
		m_DefaultSurface = AssetManager::GetMaterial("Default3D");

		//Init Cubemap
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
		
		m_CubemapBindings = ShaderBindings::Create(cubemapShader);
		m_Cubemap = Cubemap::Create("Assets/Textures/Cubemap.hdr");
		m_CubemapBindings->SetSamplerCube(m_Cubemap);


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
		PushConstantBlock transfroms("Transforms");
		Uniform uniform("u_Model", DescriptorType::Matrix, (void*)glm::value_ptr(mvp));
		transfroms.Push(uniform);
		m_CubemapBindings->SetPushConstantBlock(transfroms);

		//TODO: Submit should not need to know about the number of indices, this should be handled by the index buffer
		RenderCommand::Submit(m_CubemapVAO, m_CubemapBindings);
	}

	void SceneRenderer::EndScene()
	{
		RenderCommand::EndRenderPass();
	}

}