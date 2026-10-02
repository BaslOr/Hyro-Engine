#pragma once
#include <vector>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/VertexArray.h"
#include <cstdint>

namespace Hyro {

    struct PBRMesh {
    public:
        PBRMesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, Ref<Texture> albedo, Ref<Texture> normal, Ref<Texture> AmbientOcclusion, Ref<Texture> Rougness);

        Ref<VertexArray> VAO;
        Ref<Texture> Albedo;
		Ref<Texture> Normal;
		Ref<Texture> AmbientOcclusion;
		Ref<Texture> Roughness;
        uint32_t Count;

    private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

    struct Sprite {
		Ref<Texture> Sprite;
    };

}
