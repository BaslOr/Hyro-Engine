#include "pch.h"
#include "Hyro/Renderer/RenderCommand.h"


namespace Hyro {

	void RenderCommand::Init(GraphicsAPIType type, const GraphicsPipelineSettings& pipelineSettings)
	{
		m_API = GraphicsAPI::Create(type, pipelineSettings);
	}

	void RenderCommand::DrawIndexed(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count)
	{
		if (Renderer::GetAPI() == GraphicsAPIType::OpenGL) {
			vertexArray->Bind();
			shader->Bind();
			m_API->DrawIndexed(count);
		}
	}

	void RenderCommand::Clear()
	{
		m_API->Clear();
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		m_API->SetClearColor(color);
	}

	void RenderCommand::Submit()
	{
		m_API->Submit();
	}

	void RenderCommand::Submit(Ref<VertexArray> vertexArray)
	{

	}

	void RenderCommand::Submit(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
	{

	}

}
