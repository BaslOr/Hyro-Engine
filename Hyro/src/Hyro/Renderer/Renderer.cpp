#include "pch.h"
#include "Hyro/Renderer/Renderer.h"

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
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
	}

}