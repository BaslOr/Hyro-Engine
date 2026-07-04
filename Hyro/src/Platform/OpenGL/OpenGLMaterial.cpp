#include "pch.h"
#include "Platform/OpenGL/OpenGLMaterial.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>


namespace Hyro {

	OpenGLMaterial::OpenGLMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_Shader->Bind();
		std::array<int, 16> textureSlots = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
		int location = openGLShader->GetUniformLocation("u_Textures");
		glUniform1iv(location, textureSlots.size(), textureSlots.data());
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
