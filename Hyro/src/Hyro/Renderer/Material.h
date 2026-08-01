#pragma once
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/Cubemap.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

#include <vector>


namespace Hyro {

	struct PushConstantBlock {
		std::string Name = "";
		uint32_t Size = 0;

		explicit PushConstantBlock(const std::string& name)
			: Name(name) { }

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
		virtual void SetSamplers(const std::array<Ref<Texture>, 16>& textures) = 0;
		virtual void SetSampler(const Ref<Texture>& texture, uint32_t slot) = 0;
		virtual void SetPushConstantBlock(const PushConstantBlock& block) = 0;

		virtual void SetSamplerCube(const Ref<Cubemap>& cubemap) = 0;

		virtual void Bind() = 0;
		virtual void Bind(void* commandBuffer) = 0;
	};

}
