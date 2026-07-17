#pragma once
#include "Hyro/Renderer/Material.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"

namespace Hyro {

	class OpenGLMaterial : public Material {
	public:
		OpenGLMaterial(Ref<Shader> shader);

		void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding) override;
		void SetTextures(const std::array<Ref<Texture>, 16>& textures) override;
		void SetPushConstants(const PushConstants& pushConstants) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;


	private:
		Ref<Shader> m_Shader;

		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::array<Ref<Texture>, 16> m_Textures;

		Ref<Texture> m_FallbackTexture;
	};

}
