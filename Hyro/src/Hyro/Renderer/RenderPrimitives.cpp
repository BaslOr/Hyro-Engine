#include "pch.h"
#include "Hyro/Renderer/RenderPrimitives.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/Shader.h"

namespace Hyro {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Texture> texture)
		: Sprite(texture)
	{
        m_VertexBuffer = VertexBuffer::Create(vertices);
        m_IndexBuffer = IndexBuffer::Create(indices);
        Count = indices.size();

        VAO = VertexArray::Create();

        static Ref<Shader> shader3D = AssetManager::GetShader("Default3D");
        VertexLayout vertexLayout = shader3D->GetVertexLayout();
        m_VertexBuffer->SetLayout(vertexLayout);

        VAO->AddVertexBuffer(m_VertexBuffer);
        VAO->SetIndexBuffer(m_IndexBuffer);
	}

}