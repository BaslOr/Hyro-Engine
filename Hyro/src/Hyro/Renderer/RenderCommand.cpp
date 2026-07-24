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

	void RenderCommand::Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count)
	{
		m_API->Submit(vertexArray, material, count);
	}

	void RenderCommand::SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap)
	{
		m_API->SubmitCubemap(vertexArray, material, cubemap);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		m_API->SetClearColor(color);
	}

}
