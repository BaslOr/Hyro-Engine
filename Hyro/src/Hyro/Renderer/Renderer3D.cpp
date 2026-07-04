#include "pch.h"
#include "Hyro/Renderer/Renderer3D.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/RenderCommand.h"


namespace Hyro {

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		UniformBufferData data{};
		data.Projection = transform;
		m_Data.UBO->SetData(data);
		
		m_Data.Material->SetTexture(mesh->Texture, 0);

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(mesh->Vertices);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh->Indices);
		m_Data.VAO->AddVertexBuffer(vertexBuffer);
		m_Data.VAO->SetIndexBuffer(indexBuffer);

		RenderCommand::Submit(m_Data.VAO, m_Data.Material, mesh->Indices.size());
	}

	void Renderer3D::Init()
	{
		m_Data.UBO = UniformBuffer::Create();
		m_Data.Shader = AssetManager::GetShader("Default3D");
		m_Data.Material = Material::Create(m_Data.Shader);
		m_Data.Material->SetUnifromBuffer(m_Data.UBO, 0);

		m_Data.VAO = VertexArray::Create();
	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::BeginScene()
	{
		m_Data.Material->Bind();
	}

	void Renderer3D::EndScene()
	{
	}

}
