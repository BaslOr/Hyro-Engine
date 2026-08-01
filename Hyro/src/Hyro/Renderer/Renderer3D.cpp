#include "pch.h"
#include "Hyro/Renderer/Renderer3D.h"

#include "Hyro/Project/AssetManager.h"
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"

#include <glm/gtc/type_ptr.hpp>


namespace Hyro {

	void Renderer3D::Init()
	{
		m_Data.UBO = SceneRenderer::GetRenderer3DTransformUnifromBuffer();
		m_Data.Shader = AssetManager::GetShader("Default3D");
		m_Data.Material = Material::Create(m_Data.Shader);
		m_Data.Material->SetUnifromBuffer(m_Data.UBO);
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		PushConstantBlock transforms("Transforms");;
		Uniform model("u_Model", DescriptorType::Matrix, (void*)glm::value_ptr(transform));
		transforms.Push(model);
		m_Data.Material->SetPushConstantBlock(transforms);
		m_Data.TexturesSlots[1] = mesh->Sprite;
		m_Data.Material->SetSamplers(m_Data.TexturesSlots);

		RenderCommand::Submit(mesh->VAO, m_Data.Material, mesh->Count);
	}

	void Renderer3D::BeginScene(const glm::mat4& mvp)
	{
		UniformBufferData data{};
		data.MVP = mvp;
		m_Data.UBO->SetData(data);
		std::array<Ref<Texture>, 16> textures{};
		m_Data.Material->SetSamplers(textures);
	}

	void Renderer3D::EndScene()
	{
	}

}
