#include "pch.h"
#include "Platform/OpenGL/OpenGLMaterial.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hyro/Project/AssetManager.h"

#include <numeric>

#include <glad/glad.h>


namespace Hyro {

	OpenGLMaterial::OpenGLMaterial(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_Shader->Bind();
		m_FallbackTexture = AssetManager::GetFallbackTexture();

		//Set Samplers

		m_ReflectionData = m_Shader->GetReflectionData();
		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			if (descriptor.Type == DescriptorType::Sampler) {
				std::vector<int> textureSlots(descriptor.Count);
				std::iota(textureSlots.begin(), textureSlots.end(), 0);
					
				OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
				int location = openGLShader->GetUniformLocation("u_Textures");
				glUniform1iv(location, textureSlots.size(), textureSlots.data());
			}
		}

		//Temporary Material system will be reworked after cubemaps are finally working
		OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
		openGLShader->SetUniformInt("u_Cube", 0);

		//Default texture slots
		for (size_t i = 0; i < m_Textures.size(); ++i) {
			m_Textures[i] = m_FallbackTexture;
		}
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
		m_Textures[0]->Bind(0u);
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (textures[i] != nullptr)
				m_Textures[i] = textures[i];

			m_Textures[i]->Bind(i);
		}
	}

	void OpenGLMaterial::SetTexture(const Ref<Texture>& texture, uint32_t slot)
	{
		HYRO_ASSERT(slot < 16);

		m_Textures[slot] = texture;
		texture->Bind(slot);
	}

	void OpenGLMaterial::SetPushConstants(const PushConstants& pushConstants)
	{
		OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
		openGLShader->SetUniformMat4("u_Model", pushConstants.Model);
	}

	//void OpenGLMaterial::SetUniform(const std::string& name, void* value)
	//{
	//	for (const auto& descriptor : m_ReflectionData.Descriptors) {
	//		if (descriptor.Name == name) {
	//			OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
	//			openGLShader->set
	//			return;
	//		}
	//	}

	//	HYRO_LOG_CORE_ERROR("Could not find uniform with name: {0}", name.c_str());
	//}

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
