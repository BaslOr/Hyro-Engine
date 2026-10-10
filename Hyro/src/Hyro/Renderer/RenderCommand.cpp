#include "pch.h"
#include "Hyro/Renderer/RenderCommand.h"


namespace Hyro {

	void RenderCommand::Init(GraphicsAPIType type)
	{
		m_API = GraphicsAPI::Create(type);
	}

	void RenderCommand::BeginRenderPass()
	{
		m_API->BeginRenderPass();
	}

	void RenderCommand::EndRenderPass() {
		m_API->EndRenderPass();
	}

	void RenderCommand::Submit(Ref<VertexArray> vertexArray, Ref<ShaderBindings> material)
	{
		m_API->Submit(vertexArray, material);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		m_API->SetClearColor(color);
	}

}
