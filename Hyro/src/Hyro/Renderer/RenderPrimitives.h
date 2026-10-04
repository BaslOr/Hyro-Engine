#pragma once
#include <vector>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/VertexArray.h"
#include "Hyro/Renderer/Shader.h"

#include <cstdint>

namespace Hyro {

    struct Mesh {
    public:
        Mesh(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices);

        Ref<VertexArray> VAO;
        uint32_t Count;

    private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

    struct Sprite {
		Ref<Texture> Sprite;
    };


	struct PBRMaterialData {
        glm::vec4 BaseColor = glm::vec4(1.0f);
        float Metallic = 0.0f;
        float Roughness = 0.5f;
        float AO = 1.0f;
	};

    class Material {
	public:
        static Ref<Material> Create(Ref<Shader> shader) {
			return std::move(CreateRef<Material>(shader));
        }

		const Ref<Shader>& GetShader() const { return m_Shader; }
		const PBRMaterialData& GetMaterialData() const { return m_MaterialData; }

		const Ref<Texture>& GetAlbedo() const { return m_Albedo; }
		const Ref<Texture>& GetNormal() const { return m_Normal; }
		const Ref<Texture>& GetRoughness() const { return m_Roughness; }
		const Ref<Texture>& GetAmbientOcclusion() const { return m_AmbientOcclusion; }

		void SetAlbedo(const Ref<Texture>& albedo) { m_Albedo = albedo; m_Revision++; }
		void SetNormal(const Ref<Texture>& normal) { m_Normal = normal; m_Revision++; }
		void SetRoughness(const Ref<Texture>& roughness) { m_Roughness = roughness; m_Revision++; }
		void SetAmbientOcclusion(const Ref<Texture>& ao) { m_AmbientOcclusion = ao; m_Revision++; }

		Material(Ref<Shader>& shader)
        {
        }

	private:
		Ref<Shader> m_Shader;
		PBRMaterialData m_MaterialData;

        Ref<Texture> m_Albedo;
        Ref<Texture> m_Normal;
        Ref<Texture> m_Roughness;
        Ref<Texture> m_AmbientOcclusion;

		uint32_t m_Revision = 0;
    };

}
