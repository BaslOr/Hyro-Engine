#include "pch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"

namespace Hyro {

	void Renderer2D::Init()
	{
		m_Data.Shader = Shader::Create("Assets/Shaders/vertex.glsl", "Assets/Shaders/fragment.glsl");
		m_Data.VAO = VertexArray::Create();
		m_Data.VBO = VertexBuffer::Create(m_Data.MaxVerticesCount * sizeof(Vertex));//It may be possible that this won't work when implementing dynamic Vertex Layout in the future
		m_Data.Vertices.resize(m_Data.MaxVerticesCount);
		m_Data.IBO = IndexBuffer::Create(m_Data.MaxIndicesCount * sizeof(uint32_t));
		m_Data.Indices.resize(m_Data.MaxIndicesCount);
		m_Data.UBO = UniformBuffer::Create();

		if (m_Data.VAO != nullptr) {
			m_Data.VAO->SetIndexBuffer(m_Data.IBO);
			m_Data.VAO->AddVertexBuffer(m_Data.VBO);
			m_Data.VAO->SetVertexAttribPointer(0, 3, VertexArray::TYPE::FLOAT, 9 * sizeof(float), 0);
			m_Data.VAO->SetVertexAttribPointer(1, 2, VertexArray::TYPE::FLOAT, 9 * sizeof(float), offsetof(Vertex, UV));
			m_Data.VAO->SetVertexAttribPointer(2, 4, VertexArray::TYPE::FLOAT, 9 * sizeof(float), offsetof(Vertex, Color));
			m_Data.VAO->EnableVertexAttribArray(0);
			m_Data.VAO->EnableVertexAttribArray(1);
			m_Data.VAO->EnableVertexAttribArray(2);
		}

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
		glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f);
		m_Data.Shader->setUniformMat4("u_ProjectionMatrix", projection);

		UniformBufferData data{};
		data.Projection = projection;
		m_Data.UBO->SetData(data);

		RenderCommand::Submit(m_Data.VAO, m_Data.UBO, m_Data.Shader, static_cast<uint32_t>(m_Data.Indices.size()));
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
			color.r, color.g, color.b, color.a });
		//Top, Left
		m_Data.Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a });
		//Top, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a });
		//Bottom, Right
		m_Data.Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a });

		m_Data.Indices.push_back(0 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);
		m_Data.Indices.push_back(1 + m_Data.Count);
		m_Data.Indices.push_back(2 + m_Data.Count);
		m_Data.Indices.push_back(3 + m_Data.Count);

		m_Data.Count += 4;
	}

}
