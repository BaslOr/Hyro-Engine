#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/Renderer2D.h"

namespace Hyro {
	void Renderer::Init(const Settings& settings)
	{
		//Set Default Vertex Layout

		//Some kind of API description should be passed here
		//To determine blend func, sample count, ...
		m_GraphicsAPI = GraphicsAPI::Create(m_GraphicsAPIType);

		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene()
	{
		Renderer2D::BeginScene();
	}

	void Renderer::EndScene()
	{
		Renderer2D::EndScene();
	}

	void Renderer::DrawIndexed(uint32_t count)
	{
		m_GraphicsAPI->DrawIndexed(count);
	}

	void Renderer::Clear()
	{
		m_GraphicsAPI->Clear();
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		m_GraphicsAPI->SetClearColor(color);
	}

}