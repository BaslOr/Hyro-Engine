#include "pch.h"
#include "Hyro/Renderer/Renderer2D.h"

#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"
#include "Hyro/Core/Application.h"
#include "Hyro/Project/AssetManager.h"

#include "Hyro/Renderer/Vertex.h"

namespace Hyro {

	void Renderer2D::Init()
	{
		m_Data.Shader = AssetManager::GetShader("Default2D");
		m_Data.VAO = VertexArray::Create();
		m_Data.VBO = VertexBuffer::Create(m_Data.Shader->GetVertexLayout(), m_Data.MaxVerticesCount);
		m_Data.Vertices.resize(m_Data.MaxVerticesCount);
		m_Data.IBO = IndexBuffer::Create(m_Data.MaxIndicesCount * sizeof(uint32_t));
		m_Data.Indices.resize(m_Data.MaxIndicesCount);

		m_Data.UBO = Renderer::GetRenderer2DTransformUnifromBuffer();

		m_Data.VAO->AddVertexBuffer(m_Data.VBO);
		m_Data.VAO->SetIndexBuffer(m_Data.IBO);


		m_Data.Material = Material::Create(m_Data.Shader);
		m_Data.Material->SetUnifromBuffer(m_Data.UBO, 0);

		RenderCommand::SetClearColor(glm::vec4(0.2f, 0.5f, 0.8f, 1.f));
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const glm::mat4& projection)
	{
		m_Data.Vertices.clear();
		m_Data.Indices.clear();
		m_Data.Count = 0;

		UniformBufferData data{};
		data.MVP = projection;
		m_Data.UBO->SetData(data);
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		m_Data.VBO->SetData(m_Data.Vertices);
		m_Data.IBO->SetData(m_Data.Indices);
		PushConstants pushConstants{};
		pushConstants.Model = glm::mat4(1.0f);
		m_Data.Material->SetTextures(m_Data.Textures);
		m_Data.Material->SetPushConstants(pushConstants);

		RenderCommand::Submit(m_Data.VAO, m_Data.Material, static_cast<uint32_t>(m_Data.Indices.size()));
	}

	void Renderer2D::DrawQuadWithTextureIndex(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float textureIndex)
	{
		if (m_Data.Vertices.size() + 4 > m_Data.MaxVerticesCount) {
			Flush();
		}
		if (m_Data.Indices.size() + 6 > m_Data.MaxIndicesCount) {
			Flush();
		}

		//Bottom, Left
		m_Data.Vertices.push_back({ position.x, position.y, 0.0f,
			0.f, 0.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Top, Left
		m_Data.Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Top, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Bottom, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a,
			textureIndex });

		m_Data.Indices.push_back(0 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(2 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);

		m_Data.Count += 4;
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuadWithTextureIndex(position, size, color, 0);
	}

	void Renderer2D::DrawSprite(const Ref<Sprite>& sprite, const glm::vec2& position, const glm::vec2& size)
	{
		int textureIndex = GetSlotOfTexture(sprite->Sprite);

		if (textureIndex == 0) {
			if (m_Data.CurrentTextureSlot > m_Data.MaxTextureSlots) {
				Flush();//Flush if there is no space in TextureSlots left
			}

			textureIndex = static_cast<int>(++m_Data.CurrentTextureSlot);
			m_Data.Textures[textureIndex] = sprite->Sprite;
		}

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		DrawQuadWithTextureIndex(position, size, color, textureIndex);
	}

	uint32_t Renderer2D::GetSlotOfTexture(const Ref<Texture>& texture)
	{
		for (int i = 0; i < m_Data.MaxTextureSlots; ++i) {
			if (m_Data.Textures[i] == texture) {
				return i;
			}
		}

		return 0;
	}

}
