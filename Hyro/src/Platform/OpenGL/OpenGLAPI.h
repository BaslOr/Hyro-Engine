#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hyro {
	
	class OpenGLAPI : public GraphicsAPI {
	public:
		OpenGLAPI();

		void BeginRenderPass() override;
		void EndRenderPass() override;

		void Submit(Ref<VertexArray> vertexArray, Ref<ShaderBindings> material, uint32_t count) override;

		void SetClearColor(const glm::vec4& color) override;

	private:
		struct FramebufferVertex {
			glm::vec2 Position;
			glm::vec2 TexCoords;
		};
		struct FramebufferData {
			OpenGLFramebuffer Framebuffer;
			Ref<Texture> ColorAttachment;

			Ref<VertexArray> VertexArray;
			Ref<VertexBuffer> VertexBuffer;
			Ref<IndexBuffer> IndexBuffer;

			Ref<Shader> Shader;
			Ref<ShaderBindings> Material;
		};
		FramebufferData m_FramebufferData;

	private:
		void SetupDebugCallback();

	};

}