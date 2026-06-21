#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer2D.h"

namespace Hyro {
	void Renderer::Init(const Settings& settings)
	{
		//Set Default Vertex Layout

		//Some kind of API description should be passed here
		//To determine blend func, sample count, ...
		RenderCommand::Init(m_GraphicsAPIType);
		Renderer2D::Init(settings.PipelineSettings);
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene()
	{
		RenderCommand::BeginScene();

		Renderer2D::BeginScene();
	}

	void Renderer::EndScene()
	{
		Renderer2D::EndScene();

		RenderCommand::EndScene();
	}

}