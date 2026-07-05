#include "pch.h"
#include "Hyro/Renderer/Renderer3D.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"


namespace Hyro {

	void Renderer3D::Init()
	{
		m_Data.UBO = Renderer::GetTransfromUniformBuffer();
		m_Data.Shader = AssetManager::GetShader("Default3D");
		m_Data.Material = Material::Create(m_Data.Shader);
		m_Data.Material->SetUnifromBuffer(m_Data.UBO, 0);
	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		UniformBufferData data{};
		data.Projection = transform;
		m_Data.UBO->SetData(data);

		m_Data.Material->SetTexture(mesh->Texture, 0);

		RenderCommand::Submit(mesh->VAO, m_Data.Material, mesh->Count);
	}

	void Renderer3D::BeginScene()
	{
	}

	void Renderer3D::EndScene()
	{
	}

}
