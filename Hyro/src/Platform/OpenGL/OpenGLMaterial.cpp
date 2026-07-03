#include "pch.h"
#include "OpenGLMaterial.h"


namespace Hyro {

	OpenGLMaterial::OpenGLMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{

	}

	void OpenGLMaterial::SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding)
	{
		m_UniformBuffers[binding] = uniformBuffer;
	}

	void OpenGLMaterial::SetTexture(Ref<Texture> texture, uint32_t slot)
	{
		m_Textures[slot] = texture;
		texture->Bind(slot);
	}

	void OpenGLMaterial::Bind()
	{
		for (auto& [binding, ubo] : m_UniformBuffers)
		{
			ubo->Bind();
		}
		for (auto& [slot, texture] : m_Textures)
		{
			texture->Bind(slot);
		}
	}

	void OpenGLMaterial::Bind(void* commandBuffer)
	{
		HYRO_LOG_CORE_WARN("OpenGLMaterial::Bind(void* commandBuffer) is not implemented. Command buffers are not used in OpenGL.");
	}

}
