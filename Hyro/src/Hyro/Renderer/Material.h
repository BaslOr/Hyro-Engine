#pragma once
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

namespace Hyro {

	class Material {
	public:
		static Ref<Material> Create(Ref<Shader> shader);

		virtual void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding) = 0;
		virtual void SetTextures(const std::array<Ref<Texture>, 16>& textures) = 0;

		virtual void SetUniform(const std::string& name, void* value) = 0;
		virtual void SetTexture(const Ref<Texture>& texture, uint32_t slot) = 0;

		virtual void SetPushConstants(const PushConstants& pushConstatns) = 0;


		virtual void Bind() = 0;
		virtual void Bind(void* commandBuffer) = 0;
	};

}
