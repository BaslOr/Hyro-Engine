#include "pch.h"
#include "Hyro/Renderer/Renderer3D.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/RenderCommand.h"

#include <glm/gtc/type_ptr.hpp>


namespace Hyro {

	void Renderer3D::Init()
	{
		m_Data.Shader = AssetManager::GetShader("PBR");
		m_Data.ShaderBinding = ShaderBindings::Create(m_Data.Shader);

		m_Data.TransformBuffer = m_Data.ShaderBinding->RetrieveUniformBuffer("transform");
		m_Data.MaterialBuffer = m_Data.ShaderBinding->RetrieveUniformBuffer("material");
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const Ref<Material>& surface, const glm::mat4& transform)
	{
		PushConstantBlock transforms("transform");
		Uniform model("u_Model", DescriptorType::Matrix, (void*)glm::value_ptr(transform));
		transforms.Push(model);

		m_Data.ShaderBinding->SetPushConstantBlock(transforms);
		if (surface->GetRevisions() != 0)
			m_Data.MaterialBuffer->SetData((void*)&surface->GetMaterialData());

		uint32_t albedoSlot = m_Data.ShaderBinding->GetNextTextureSlotIndex(surface->GetAlbedo());
		uint32_t normalSlot = m_Data.ShaderBinding->GetNextTextureSlotIndex(surface->GetNormal());
		uint32_t roughnessSlot = m_Data.ShaderBinding->GetNextTextureSlotIndex(surface->GetRoughness());
		uint32_t metallicSlot = roughnessSlot; // The 3D model features a metallic rouughness texture not two seperate textures
		uint32_t aoSlot = m_Data.ShaderBinding->GetNextTextureSlotIndex(surface->GetAmbientOcclusion());
		struct alignas(16) TextureSlots {
			uint32_t AlbedoSlot;
			uint32_t NormalSlot;
			uint32_t MetallicSlot;
			uint32_t RoughnessSlot;
			uint32_t AmbientOcclusionSlot;
		};

		TextureSlots slots = {
			albedoSlot,
			normalSlot,
			metallicSlot,	
			roughnessSlot,
			aoSlot
		};
		static_assert(sizeof(TextureSlots) == 32);
		static_assert(offsetof(TextureSlots, MetallicSlot) == 8);
		static_assert(offsetof(TextureSlots, AmbientOcclusionSlot) == 16);

		m_Data.MaterialBuffer->SetData(&slots);

		RenderCommand::Submit(mesh->VAO, m_Data.ShaderBinding);
	}

	void Renderer3D::BeginScene(const glm::mat4& ViewProjection)
	{
		TransformData data{};
		data.MVP = ViewProjection;
		m_Data.TransformBuffer->SetData(&data);
	}

	void Renderer3D::EndScene()
	{
	}

}
