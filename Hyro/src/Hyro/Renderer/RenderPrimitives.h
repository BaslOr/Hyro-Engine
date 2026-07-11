#pragma once
#include <vector>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/Texture.h"
#include "Hyro/Renderer/VertexArray.h"

namespace Hyro {

    struct Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Texture> texture)
            : Texture(texture)
        {
            m_VertexBuffer = VertexBuffer::Create(vertices);
            m_IndexBuffer = IndexBuffer::Create(indices);
            Count = indices.size();

            VAO = VertexArray::Create();

            VertexLayout vertexLayout{};
            vertexLayout.Push<VertexAttributeType::FLOAT3>();
            vertexLayout.Push<VertexAttributeType::FLOAT2>();
            vertexLayout.Push<VertexAttributeType::FLOAT4>();
            vertexLayout.Push<VertexAttributeType::FLOAT>();
            m_VertexBuffer->SetLayout(vertexLayout);
            VAO->AddVertexBuffer(m_VertexBuffer);
            VAO->SetIndexBuffer(m_IndexBuffer);
        }

        Ref<VertexArray> VAO;
        Ref<Texture> Texture;
        uint32_t Count;

    private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

    struct Sprite {
		Ref<Texture> Texture;
    };

}
