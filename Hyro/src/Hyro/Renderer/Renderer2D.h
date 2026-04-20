#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Vertex.h"
#include "Hyro/Core/Window.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	struct Renderer2DData {
		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
		Ref<Shader> Shader;

		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		uint32_t Count = 0;
	};

	class Renderer2D {
	public:


		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	private:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

	private:
		inline static Renderer2DData m_Data;

		friend Renderer;
	};

}
