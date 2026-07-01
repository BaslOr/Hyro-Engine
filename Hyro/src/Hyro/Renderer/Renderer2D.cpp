#include "pch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"
#include "Hyro/Core/Application.h"

namespace Hyro {

	void Renderer2D::Init(const GraphicsPipelineSettings& settings)
	{
		m_Data.VAO = VertexArray::Create();
		m_Data.VBO = VertexBuffer::Create(m_Data.MaxVerticesCount * sizeof(Vertex));//It may be possible that this won't work when implementing dynamic Vertex Layout in the future
		m_Data.Vertices.resize(m_Data.MaxVerticesCount);
		m_Data.IBO = IndexBuffer::Create(m_Data.MaxIndicesCount * sizeof(uint32_t));
		m_Data.Indices.resize(m_Data.MaxIndicesCount);

		m_Data.UBO = UniformBuffer::Create();

		m_Data.VAO->AddVertexBuffer(m_Data.VBO);
		m_Data.VAO->SetIndexBuffer(m_Data.IBO);

		VertexLayout vertexLayout{};
		vertexLayout.Push<VertexAttributeType::FLOAT3>();
		vertexLayout.Push<VertexAttributeType::FLOAT2>();
		vertexLayout.Push<VertexAttributeType::FLOAT4>();
		vertexLayout.Push<VertexAttributeType::FLOAT>();
		m_Data.VAO->SetLayout(vertexLayout);


		m_Data.Shader = Shader::Create(settings);
		m_Data.Material = Material::Create(m_Data.Shader);
		m_Data.Material->SetUnifromBuffer(m_Data.UBO, 0);

		RenderCommand::SetClearColor(glm::vec4(0.2f, 0.5f, 0.8f, 1.f));


		HYRO_LOG_CORE_TRACE("Initialized Renderer");
	}

	void Renderer2D::Shutdown()
	{
		HYRO_LOG_CORE_TRACE("Destroyed Renderer");
	}

	void Renderer2D::BeginScene()
	{
		m_Data.Vertices.clear();
		m_Data.Indices.clear();
		m_Data.Count = 0;
	}

	void Renderer2D::EndScene()
	{
		m_Data.VBO->SetData(m_Data.Vertices);
		m_Data.IBO->SetData(m_Data.Indices);

		//TODO: Get size from Framebuffer
		Application& app = Application::Get();
		float width = static_cast<float>(app.GetWindow()->GetWidth());
		float height = static_cast<float>(app.GetWindow()->GetHeight());

		UniformBufferData data;
		if (Renderer::GetAPI() == GraphicsAPIType::Vulkan)
			data.Projection = glm::ortho(0.f, width, height, 0.f);
		else if (Renderer::GetAPI() == GraphicsAPIType::OpenGL)
			data.Projection = glm::ortho(0.f, width, 0.f, height);
		m_Data.UBO->SetData(data);

		RenderCommand::Submit(m_Data.VAO, m_Data.Material, static_cast<uint32_t>(m_Data.Indices.size()));
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (m_Data.Vertices.size() + 4 > m_Data.MaxVerticesCount) {
			HYRO_ASSERT(false);
		}
		if (m_Data.Indices.size() + 6 > m_Data.MaxIndicesCount) {
			HYRO_ASSERT(false);
		}

		//Bottom, Left
		m_Data.Vertices.push_back({ position.x, position.y, 0.0f,
			0.f, 0.f,
			color.r, color.g, color.b, color.a,
			1.0f});
		//Top, Left
		m_Data.Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a,
			1.0f});
		//Top, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a,
			1.0f});
		//Bottom, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a,
			1.0f});

		m_Data.Indices.push_back(0 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(2 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);

		m_Data.Count += 4;
	}

	void Renderer2D::DrawSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size)
	{
		int textureIndex = 0;

		//Check if Texture is already in the slots
		for (uint32_t i = 1; i < 16; i++)
		{
			if (m_Data.TexturesSlots[i] == texture) {
				textureIndex = i;
				m_Data.Material->SetTexture(texture, textureIndex);
				break;
			}
		}

		if (textureIndex == 0) {
			if (textureIndex >= 15) {
				//TODO: Flush and reset batch
				HYRO_ASSERT(false);
			}

			textureIndex = static_cast<float>(m_Data.CurrentTextureSlot);
			m_Data.TexturesSlots[m_Data.CurrentTextureSlot] = texture;
			m_Data.CurrentTextureSlot++;
			m_Data.Material->SetTexture(texture, static_cast<uint32_t>(textureIndex));
		}

		m_Data.Material->SetTexture(texture, static_cast<uint32_t>(textureIndex));

		if (m_Data.Vertices.size() + 4 > m_Data.MaxVerticesCount) {
			HYRO_ASSERT(false);
		}
		if (m_Data.Indices.size() + 6 > m_Data.MaxIndicesCount) {
			HYRO_ASSERT(false);
		}

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		//Bottom, Left
		m_Data.Vertices.push_back({ position.x, position.y, 0.0f,
			0.f, 0.f,
			color.r, color.g, color.b, color.a,
			static_cast<float>(textureIndex) });
		//Top, Left
		m_Data.Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a,
			static_cast<float>(textureIndex) });
		//Top, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a,
			static_cast<float>(textureIndex) });
		//Bottom, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a,
			static_cast<float>(textureIndex) });

		m_Data.Indices.push_back(0 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(2 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);
		
		m_Data.Count += 4;
	}

}
