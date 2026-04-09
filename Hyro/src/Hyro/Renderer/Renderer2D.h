#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Vertex.h"
#include "Hyro/Core/Window.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	class Renderer2D {
	public:
		Renderer2D(std::shared_ptr<Window> window);
		~Renderer2D();

		void BeginScene();
		void EndScene();

		void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	private:
		std::shared_ptr<Window> m_Window;

		Scope<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;
		Ref<Shader> m_Shader;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		uint32_t m_Count = 0;
	};

}
