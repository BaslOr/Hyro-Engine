#pragma once
#include <array>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/VertexArray.h"
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/Texture.h"
#include "Hyro/Renderer/UniformBuffer.h"
#include "Hyro/Renderer/Material.h"
#include "Hyro/Renderer/RenderPrimitives.h"

namespace Hyro {

	struct Renderer2DData {
		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;

		size_t CurrentTextureSlot = 0;
		std::array<Ref<Texture>, 16> TexturesSlots;

		Ref<Shader> Shader;
		Ref<UniformBuffer> UBO;
		Ref<Material> Material;

		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		uint32_t Count = 0;

		uint32_t MaxVerticesCount = 2000;
		uint32_t MaxIndicesCount = 6000;
	};


	class Renderer2D {
	public:
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		static void DrawSprite(const Ref<Sprite>& sprite, const glm::vec2& position, const glm::vec2& size);

	private:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& projection);
		static void EndScene();

	private:
		inline static Renderer2DData m_Data;

		friend class Scene;
		friend class Renderer;
	};

}
