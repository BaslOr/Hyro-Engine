#include "pch.h"
#include "Platform/OpenGL/OpenGLMaterial.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hyro/Project/AssetManager.h"

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

		m_FallbackTexture = AssetManager::GetFallbackTexture();
	}

	void OpenGLMaterial::SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding)
	{
		m_UniformBuffers[binding] = uniformBuffer;
		OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
		uint32_t uniformBlockIndex = glGetUniformBlockIndex(openGLShader->GetProgram(), "UniformBufferObject");
		glUniformBlockBinding(openGLShader->GetProgram(), uniformBlockIndex, 0);
	}

	void OpenGLMaterial::SetTextures(const std::array<Ref<Texture>, 16>& textures)
	{
		m_Shader->Bind();
		m_Textures[0] = m_FallbackTexture;
		m_Textures[0]->Bind(0U);
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (textures[i] != nullptr)
				m_Textures[i] = textures[i];
			else
				m_Textures[i] = m_FallbackTexture;

			m_Textures[i]->Bind(i);
		}
	}

	void OpenGLMaterial::SetPushConstants(const PushConstants& pushConstants)
	{
		OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
		openGLShader->SetUniformMat4("u_Model", pushConstants.Model);
	}

	void OpenGLMaterial::Bind()
	{
		m_Shader->Bind();
		for (auto& [binding, ubo] : m_UniformBuffers)
		{
			ubo->Bind();
		}

		for (size_t i = 0; i < m_Textures.size(); ++i) {
			m_Textures[i]->Bind(i);
		}
	}

	void OpenGLMaterial::Bind(void* commandBuffer)
	{
		HYRO_LOG_CORE_WARN("OpenGLMaterial::Bind(void* commandBuffer) is not implemented. Command buffers are not used in OpenGL.");
	}

}
