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

	void OpenGLMaterial::SetTexture(Ref<Texture> texture, uint32_t binding, uint32_t slot)
	{
		m_Textures[binding] = texture;
		texture->Bind();
	}

	void OpenGLMaterial::Bind()
	{
		for (auto& [binding, ubo] : m_UniformBuffers)
		{
			ubo->Bind();
		}
		for (auto& [binding, texture] : m_Textures)
		{
			texture->Bind();
		}
	}

}
