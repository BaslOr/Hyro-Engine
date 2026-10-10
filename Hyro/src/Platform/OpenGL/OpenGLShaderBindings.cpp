#include "pch.h"
#include "Platform/OpenGL/OpenGLShaderBindings.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCubemap.h"

#include "Hyro/Project/AssetManager.h"

#include <numeric>

#include <glad/glad.h>


namespace Hyro {

	OpenGLShaderBindings::OpenGLShaderBindings(Ref<Shader> shader)
		: m_Shader(shader)
	{
		m_Shader->Bind();

		m_FallbackTexture = AssetManager::GetFallbackTexture();
		m_ReflectionData = m_Shader->GetReflectionData();
		m_Textures.resize(0);
		
		for (const auto& descriptor : m_ReflectionData.Descriptors) {
			if (descriptor.Type == DescriptorType::UniformBuffer) {
				if (s_UniformBuffersByBinding.find(descriptor.Binding) == s_UniformBuffersByBinding.end()) {
					Ref<UniformBuffer> ubo = UniformBuffer::Create(descriptor.Binding, descriptor.BlockSize);
					s_UniformBuffersByBinding[descriptor.Binding] = ubo;
					s_UniformBuffersByName[descriptor.Name] = ubo;
				}
			}
			else if (descriptor.Type == DescriptorType::Sampler) {
				m_Textures.resize(16);
				std::vector<int> textureSlots(descriptor.Count);
				std::iota(textureSlots.begin(), textureSlots.end(), 0);
				OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
				int location = openGLShader->GetUniformLocation(descriptor.Name);
				glUniform1iv(location, textureSlots.size(), textureSlots.data());

				for (size_t i = 0; i < m_Textures.size(); ++i) {
					m_Textures[i] = m_FallbackTexture;
				}
			}
			else if (descriptor.Type == DescriptorType::SamplerCube) {
				OpenGLShader* openGLShader = static_cast<OpenGLShader*>(m_Shader.get());
				openGLShader->SetUnifrom({ descriptor.Name, DescriptorType::Sampler, 0 });
				m_HasCubemapSampler = true;
			}
		}

		bool hasSamplers = m_Textures.size() > 0;
		if (hasSamplers && m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Shader has both a sampler and a cubemap sampler. This is not supported. This may indicate a bug.");
		}
	}

	Ref<UniformBuffer> OpenGLShaderBindings::RetrieveUniformBuffer(const std::string& name) const
	{
		if (s_UniformBuffersByName.find(name) != s_UniformBuffersByName.end())
			return s_UniformBuffersByName[name];

		HYRO_ASSERT(false, "Failed to find Uniform Buffer with name: {}", name.c_str());
		return nullptr;
	}

	uint32_t OpenGLShaderBindings::GetNextTextureSlotIndex(Ref<Texture> texture)
	{
		if (texture == nullptr) {
			HYRO_LOG_CORE_ERROR("Tried to get a texture slot for a null texture. This may indicate a bug.");
			return 0;
		}

		if (IsTextureBound(texture))
			return std::distance(m_Textures.begin(), std::find(m_Textures.begin(), m_Textures.end(), texture));

		if (GetFreeTextureSlotCount() == 0) {
			HYRO_LOG_CORE_ERROR("Tried to get a texture slot for a texture but all slots are full. This may indicate a bug.");
			return 0;
		}

		uint32_t slot = 1;
		while (slot < m_Textures.size() && m_Textures[slot] != m_FallbackTexture) {
			slot++;
		}
		m_Textures[slot] = texture;
		return slot;
	}

	uint32_t OpenGLShaderBindings::GetFreeTextureSlotCount() const
	{
		uint32_t freeSlots = 0;
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (m_Textures[i] == m_FallbackTexture)
				freeSlots++;
		}

		return freeSlots;
	}

	bool OpenGLShaderBindings::IsTextureBound(Ref<Texture> texture) const
	{
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			if (m_Textures[i] == texture)
				return true;
		}

		return false;
	}

	void OpenGLShaderBindings::FlushTextureSlots()
	{
		for (size_t i = 1; i < m_Textures.size(); ++i) {
			m_Textures[i] = m_FallbackTexture;
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

	void OpenGLShaderBindings::SetSamplerCube(const Ref<Cubemap>& cubemap)
	{
		if (!m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Tried to set a cubemap but the shader does not have a cubemap sampler. This may indicate a bug.");
			return;
		}

		m_Cubemap = cubemap;
		m_IsCubemapSet = true;
	}

	void OpenGLShaderBindings::Bind()
	{
		if (!m_IsCubemapSet && m_HasCubemapSampler) {
			HYRO_LOG_CORE_ERROR("Tried to bind Material without a Cubemap but the shader has a cubemap sampler. This may indicate a bug.");
			return;
		}

		m_Shader->Bind();
		for (auto& [binding, ubo] : s_UniformBuffersByBinding)
		{
			ubo->Bind();
		}

		for (size_t i = 0; i < m_Textures.size(); ++i) {
			// Texture::Bind(uint32_t slot) should not be public
			m_Textures[i]->Bind(i);
		}

		if (m_IsCubemapSet) {
			m_Cubemap->Bind();
		}
	}

	void OpenGLShaderBindings::Bind(void* commandBuffer)
	{
		HYRO_LOG_CORE_WARN("OpenGLMaterial::Bind(void* commandBuffer) is not implemented. Command buffers are not used in OpenGL.");
	}

}
