#include "pch.h"
#include "Hyro/Renderer/RenderCommand.h"


namespace Hyro {

	void RenderCommand::Init(GraphicsAPIType type)
	{
		m_API = GraphicsAPI::Create(type);
	}

	void RenderCommand::BeginScene()
	{
		m_API->BeginScene();
	}

	void RenderCommand::EndScene()
	{
		m_API->EndScene();
	}

	void RenderCommand::Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count)
	{
		m_API->Submit(vertexArray, shader, count);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		m_API->SetClearColor(color);
	}

}
