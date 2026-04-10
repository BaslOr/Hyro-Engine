#include "pch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hyro/Renderer/Renderer.h"

namespace Hyro {

	Renderer2D::Renderer2D(Ref<Window> window)
		: m_Window(window)
	{
		m_Shader = Shader::Create("Assets/Shaders/vertex.glsl", "Assets/Shaders/fragment.glsl");
		m_VAO = VertexArray::Create();
		m_VBO = VertexBuffer::Create();
		m_IBO = IndexBuffer::Create();

		m_VAO->SetIndexBuffer(m_IBO);
		m_VAO->AddVertexBuffer(m_VBO);
		m_VAO->SetVertexAttribPointer(0, 3, VertexArray::TYPE::FLOAT, 9 * sizeof(float), 0);
		m_VAO->SetVertexAttribPointer(1, 4, VertexArray::TYPE::FLOAT, 9 * sizeof(float), offsetof(Vertex, Color));
		m_VAO->SetVertexAttribPointer(2, 2, VertexArray::TYPE::FLOAT, 9 * sizeof(float), offsetof(Vertex, UV));
		m_VAO->EnableVertexAttribArray(0);
		m_VAO->EnableVertexAttribArray(1);
		m_VAO->EnableVertexAttribArray(2);
	}

	Renderer2D::~Renderer2D()
	{
		HYRO_LOG_CORE_TRACE("Destroyed Renderer");
	}

	void Renderer2D::BeginScene()
	{
		Renderer::SetClearColor(glm::vec4(0.2f, 0.5f, 0.8f, 1.f));
		Renderer::Clear();

		m_Vertices.clear();
		m_Indices.clear();
		m_Count = 0;
	}

	void Renderer2D::EndScene()
	{
		m_VBO->SetData(m_Vertices);
		m_IBO->SetData(m_Indices);

		m_VAO->Bind();
		m_Shader->Bind();
		float width = static_cast<float>(m_Window->GetWidth());
		float height = static_cast<float>(m_Window->GetHeight());
		glm::mat4 projection = glm::ortho(0.f, width, 0.f, height);
		m_Shader->setUniformMat4("u_ProjectionMatrix", projection);

		Renderer::DrawIndexed(static_cast<uint32_t>(m_Indices.size()));
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		//Bottom, Left
		m_Vertices.push_back({ position.x, position.y, 0.0f,
			0.f, 0.f,
			color.r, color.g, color.b, color.a });
		//Top, Left
		m_Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a });
		//Top, Right
		m_Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a });
		//Bottom, Right
		m_Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a });

		m_Indices.push_back(0 + m_Count);
		m_Indices.push_back(1 + m_Count);
		m_Indices.push_back(3 + m_Count);
		m_Indices.push_back(1 + m_Count);
		m_Indices.push_back(2 + m_Count);
		m_Indices.push_back(3 + m_Count);

		m_Count += 4;
	}

}
