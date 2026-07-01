#pragma once
#include "Hyro/Renderer/Material.h"
#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class OpenGLMaterial : public Material {
	public:
		OpenGLMaterial(Ref<Shader> shader);

		void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding) override;
		void SetTexture(Ref<Texture> texture, uint32_t binding, uint32_t slot) override;

		void Bind() override;


	private:
		Ref<Shader> m_Shader;

		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::unordered_map<uint32_t, Ref<Texture>> m_Textures;
	};

}
