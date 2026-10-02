#include "pch.h"
#include "Hyro/Renderer/RenderPrimitives.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/Shader.h"

namespace Hyro {

	PBRMesh::PBRMesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, Ref<Texture> albedo, Ref<Texture> normal, Ref<Texture> ambientOcclusion, Ref<Texture> rougness)
		: Albedo(albedo), Normal(normal), AmbientOcclusion(ambientOcclusion), Roughness(rougness)
	{
        static Ref<Shader> shader3D = AssetManager::GetShader("PBR");

        m_VertexBuffer = VertexBuffer::Create(shader3D->GetVertexLayout(), vertices.size());
        m_VertexBuffer->SetData(vertices);
        m_IndexBuffer = IndexBuffer::Create(indices);
          Count = indices.size();

        VAO = VertexArray::Create();


        VAO->AddVertexBuffer(m_VertexBuffer);
        VAO->SetIndexBuffer(m_IndexBuffer);
	}

}