#pragma once
#include "Hyro/Renderer/ShaderBindings.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/Utils/ShaderUtils.h"
#include <string>
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"
#include <cstdint>

namespace Hyro {

	class OpenGLShaderBindings : public ShaderBindings {
	public:
		OpenGLShaderBindings(Ref<Shader> shader);

		Ref<UniformBuffer> RetrieveUniformBuffer(const std::string& name) const override;

		uint32_t GetNextTextureSlotIndex(Ref<Texture> texture);
		uint32_t GetFreeTextureSlotCount() const;
		bool IsTextureBound(Ref<Texture> texture) const;
		void FlushTextureSlots();

		void SetSamplers(const std::array<Ref<Texture>, 16>& textures) override;
		void SetSampler(const Ref<Texture>& texture, uint32_t slot) override;
		void SetPushConstantBlock(const PushConstantBlock& block) override;

		void SetSamplerCube(const Ref<Cubemap>& cubemap) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;


	private:
		Ref<Shader> m_Shader;
		ShaderReflectionData m_ReflectionData;

		static inline std::unordered_map<std::string, Ref<UniformBuffer>> s_UniformBuffersByName;
		static inline std::unordered_map<uint32_t, Ref<UniformBuffer>> s_UniformBuffersByBinding;

		std::vector<Ref<Texture>> m_Textures;
		Ref<Cubemap> m_Cubemap;

		Ref<Texture> m_FallbackTexture;

		bool m_IsCubemapSet = false;
		bool m_HasCubemapSampler = false;
	};

}
