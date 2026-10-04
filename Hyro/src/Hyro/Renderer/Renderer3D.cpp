#include "pch.h"
#include "Hyro/Renderer/Renderer3D.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/RenderCommand.h"

#include <glm/gtc/type_ptr.hpp>


namespace Hyro {

	void Renderer3D::Init()
	{
		m_Data.Shader = AssetManager::GetShader("PBR");
		m_Data.Material = ShaderBindings::Create(m_Data.Shader);

		//m_Data.MaterialBuffer = m_Data.Material->RetrieveUniformBuffer("material");
		m_Data.TransformBuffer = m_Data.Material->RetrieveUniformBuffer("transform");
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const Ref<Material>& surface, const glm::mat4& transform)
	{
		PushConstantBlock transforms("transform");
		Uniform model("u_Model", DescriptorType::Matrix, (void*)glm::value_ptr(transform));
		transforms.Push(model);

		m_Data.Material->SetPushConstantBlock(transforms);
		m_Data.TexturesSlots[1] = surface->GetAlbedo();
		m_Data.TexturesSlots[2] = surface->GetNormal();
		m_Data.TexturesSlots[3] = surface->GetRoughness();
		m_Data.TexturesSlots[4] = surface->GetAmbientOcclusion();
		m_Data.Material->SetSamplers(m_Data.TexturesSlots);

		RenderCommand::Submit(mesh->VAO, m_Data.Material, mesh->Count);
	}

	void Renderer3D::BeginScene(const glm::mat4& mvp)
	{
		TransformData data{};
		data.MVP = mvp;
		m_Data.TransformBuffer->SetData(&data);
	}

	void Renderer3D::EndScene()
	{
	}

}
