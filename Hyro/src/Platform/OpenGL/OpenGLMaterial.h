#pragma once
#include "Hyro/Renderer/Material.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/Utils/ShaderUtils.h"
#include <string>

namespace Hyro {

	class OpenGLMaterial : public Material {
	public:
		OpenGLMaterial(Ref<Shader> shader);

		void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer) override;
		void SetTextures(const std::array<Ref<Texture>, 16>& textures) override;
		void SetTexture(const Ref<Texture>& texture, uint32_t slot) override;
		void SetPushConstantBlock(const PushConstantBlock& block) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;


	private:
		Ref<Shader> m_Shader;
		ShaderReflectionData m_ReflectionData;

		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::vector<Ref<Texture>> m_Textures;

		Ref<Texture> m_FallbackTexture;
	};

}
