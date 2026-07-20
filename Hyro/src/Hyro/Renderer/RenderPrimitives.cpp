#include "pch.h"
#include "Hyro/Renderer/RenderPrimitives.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/Shader.h"

namespace Hyro {

	Mesh::Mesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, Ref<Texture> texture)
		: Sprite(texture)
	{
        static Ref<Shader> shader3D = AssetManager::GetShader("Default3D");

        m_VertexBuffer = VertexBuffer::Create(shader3D->GetVertexLayout(), vertices.size());
        m_VertexBuffer->SetData(vertices);
        m_IndexBuffer = IndexBuffer::Create(indices);
          Count = indices.size();

        VAO = VertexArray::Create();


        VAO->AddVertexBuffer(m_VertexBuffer);
        VAO->SetIndexBuffer(m_IndexBuffer);
	}

}