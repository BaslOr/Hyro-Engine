#include "pch.h"
#include "Platform/OpenGL/OpenGLShaderBindings.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hyro/Project/AssetManager.h"

#include <numeric>

#include <glad/glad.h>


namespace Hyro {

	OpenGLShaderBindings::OpenGLShaderBindings(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_Shader->Bind();
		m_FallbackTexture = AssetManager::GetFallbackTexture();

		//Set Samplers
		m_ReflectionData = m_Shader->GetReflectionData();
		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			if (descriptor.Type == DescriptorType::Sampler) {
				if (descriptor.Count > 1) {
					m_Textures.resize(16);
					std::vector<int> textureSlots(descriptor.Count);
					std::iota(textureSlots.begin(), textureSlots.end(), 0);

					OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
					int location = openGLShader->GetUniformLocation(descriptor.Name);
					glUniform1iv(location, textureSlots.size(), textureSlots.data());
				}
				else {
					m_Textures.resize(1);
					OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
					openGLShader->SetUnifrom({ descriptor.Name, DescriptorType::Sampler, 0 });
				}
			}
		}

		//Default texture slots
		for (size_t i = 0; i < m_Textures.size(); ++i) {
			m_Textures[i] = m_FallbackTexture;
		}


		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			if (descriptor.Type == DescriptorType::UniformBuffer) {
				//If UBO is not created - create it
				//Keep in mind that OpenGL uses one UniformBuffer for all Shaders
				if (s_UniformBuffersByBinding.find(descriptor.Binding) == s_UniformBuffersByBinding.end()) {
					Ref<UniformBuffer> ubo = UniformBuffer::Create(descriptor.Binding, descriptor.BlockSize);
					s_UniformBuffersByBinding[descriptor.Binding] = ubo;
					s_UniformBuffersByName[descriptor.Name] = ubo;
				}
			}
		}
	}

	Ref<UniformBuffer> OpenGLShaderBindings::RetrieveUniformBuffer(const std::string& name) const
	{
		if (s_UniformBuffersByName.find(name) != s_UniformBuffersByName.end())
			return s_UniformBuffersByName[name];

		HYRO_ASSERT(false, "Failed to find Uniform Buffer with name: {}", name.c_str());
		return nullptr;
	}

	void OpenGLShaderBindings::SetSamplers(const std::array<Ref<Texture>, 16>& textures)
	{
		m_Shader->Bind();
		m_Textures[0] = m_FallbackTexture;
		m_Textures[0]->Bind(0u);
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (textures[i] != nullptr)
				m_Textures[i] = textures[i];
			else
				m_Textures[i] = m_FallbackTexture;

			m_Textures[i]->Bind(i);
		}
	}

	void OpenGLShaderBindings::SetSampler(const Ref<Texture>& texture, uint32_t slot)
	{
		HYRO_ASSERT(slot < 16);

		m_Textures[slot] = texture;
		texture->Bind(slot);
	}

	void OpenGLShaderBindings::SetPushConstantBlock(const PushConstantBlock& block)
	{
		for (auto& unifrom : block.GetUniforms()) {
			OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
			openGLShader->SetUnifrom(unifrom);
		}
	}

	void OpenGLShaderBindings::Bind()
	{
		m_Shader->Bind();
		for (auto& [binding, ubo] : s_UniformBuffersByBinding)
		{
			ubo->Bind();
		}

		for (size_t i = 0; i < m_Textures.size(); ++i) {
			m_Textures[i]->Bind(i);
		}
	}

	void OpenGLShaderBindings::Bind(void* commandBuffer)
	{
		HYRO_LOG_CORE_WARN("OpenGLMaterial::Bind(void* commandBuffer) is not implemented. Command buffers are not used in OpenGL.");
	}

}
