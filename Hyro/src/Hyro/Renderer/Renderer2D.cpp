#include "pch.h"
#include "Hyro/Renderer/Renderer2D.h"

#include "Hyro/Renderer/RenderCommand.h"
#include "Hyro/Renderer/Renderer.h"
#include "Hyro/Core/Application.h"
#include "Hyro/Project/AssetManager.h"

#include "Hyro/Renderer/Vertex.h"

#include <glm/gtc/type_ptr.hpp>

namespace Hyro {

	void Renderer2D::Init()
	{
		s_Data.Shader = AssetManager::GetShader("Default2D");
		s_Data.VAO = VertexArray::Create();
		s_Data.VBO = VertexBuffer::Create(s_Data.Shader->GetVertexLayout(), s_Data.MaxVerticesCount);
		s_Data.Vertices.resize(s_Data.MaxVerticesCount);
		s_Data.IBO = IndexBuffer::Create(s_Data.MaxIndicesCount * sizeof(uint32_t));
		s_Data.Indices.resize(s_Data.MaxIndicesCount);


		s_Data.VAO->AddVertexBuffer(s_Data.VBO);
		s_Data.VAO->SetIndexBuffer(s_Data.IBO);


		s_Data.ShaderBindings = ShaderBindings::Create(s_Data.Shader);
		s_Data.UBO = s_Data.ShaderBindings->RetrieveUniformBuffer("transform");

		RenderCommand::SetClearColor(glm::vec4(0.2f, 0.5f, 0.8f, 1.f));
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const glm::mat4& projection)
	{
		s_Data.Vertices.clear();
		s_Data.Indices.clear();
		s_Data.Count = 0;

		TransformData data{};
		data.MVP = projection;
		s_Data.UBO->SetData(&data);
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		s_Data.VBO->SetData(s_Data.Vertices);
		s_Data.IBO->SetData(s_Data.Indices);

		PushConstantBlock transfroms("transform");
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		Uniform model("u_Model", DescriptorType::Matrix, glm::value_ptr(modelMatrix));
		transfroms.Push(model);
		s_Data.ShaderBindings->SetPushConstantBlock(transfroms);

		RenderCommand::Submit(s_Data.VAO, s_Data.ShaderBindings);

		s_Data.Vertices.clear();
		s_Data.Indices.clear();
		s_Data.Count = 0;
		s_Data.ShaderBindings->FlushTextureSlots();
	}

	void Renderer2D::DrawQuadWithTextureIndex(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float textureIndex)
	{
		if (s_Data.Vertices.size() + 4 > s_Data.MaxVerticesCount)
			Flush();
		if (s_Data.Indices.size() + 6 > s_Data.MaxIndicesCount)
			Flush();

		//Bottom, Left
		s_Data.Vertices.push_back({ position.x, position.y, 0.0f,
			0.f, 0.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Top, Left
		s_Data.Vertices.push_back({ position.x, position.y + size.y, 0.0f,
			0.f, 1.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Top, Right
		s_Data.Vertices.push_back({ position.x + size.x, position.y + size.y, 0.0f,
			1.f, 1.f,
			color.r, color.g, color.b, color.a,
			textureIndex });
		//Bottom, Right
		s_Data.Vertices.push_back({ position.x + size.x, position.y, 0.0f,
			1.f, 0.f,
			color.r, color.g, color.b, color.a,
			textureIndex });

		s_Data.Indices.push_back(0 + s_Data.Count);
		s_Data.Indices.push_back(1 + s_Data.Count);
		s_Data.Indices.push_back(3 + s_Data.Count);
		s_Data.Indices.push_back(1 + s_Data.Count);
		s_Data.Indices.push_back(2 + s_Data.Count);
		s_Data.Indices.push_back(3 + s_Data.Count);

		s_Data.Count += 4;
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuadWithTextureIndex(position, size, color, 0);
	}

	void Renderer2D::DrawSprite(const Ref<Sprite>& sprite, const glm::vec2& position, const glm::vec2& size)
	{
		if (s_Data.ShaderBindings->GetFreeTextureSlotCount() == 0)
			Flush();

		uint32_t textureIndex = s_Data.ShaderBindings->GetNextTextureSlotIndex(sprite->Sprite);
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		DrawQuadWithTextureIndex(position, size, color, textureIndex);
	}

}
