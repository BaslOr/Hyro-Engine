#pragma once
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

namespace Hyro {

	struct PushConstantBlock {
		std::string Name = "";
		uint32_t Size = 0;

		inline void Push(const Uniform& uniform) {
			m_Uniforms.push_back(uniform);

			Size += SizeOfDescriptorType(uniform.Type);
		}

		inline std::vector<Uniform> GetUniforms() const { return m_Uniforms; }

	private:
		std::vector<Uniform> m_Uniforms;
	};

	class Material {
	public:
		static Ref<Material> Create(Ref<Shader> shader);

		virtual void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer) = 0;
		virtual void SetTextures(const std::array<Ref<Texture>, 16>& textures) = 0;
		virtual void SetTexture(const Ref<Texture>& texture, uint32_t slot) = 0;
		virtual void SetPushConstantBlock(const PushConstantBlock& block) = 0;

		virtual void Bind() = 0;
		virtual void Bind(void* commandBuffer) = 0;
	};

}
