#pragma once
#include "Hyro/Renderer/ShaderBindings.h"

#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/Utils/ShaderUtils.h"
#include <cstdint>
#include <Hyro/Renderer/RenderingObjects/UniformBuffer.h>


namespace Hyro {

	class VulkanShaderBindings : public ShaderBindings {
	public:
		VulkanShaderBindings(Ref<Shader> shader);

		Ref<UniformBuffer> RetrieveUniformBuffer(const std::string& name) const override;


		uint32_t GetNextTextureSlotIndex(Ref<Texture> texture) override;
		uint32_t GetFreeTextureSlotCount() const override;
		bool IsTextureBound(Ref<Texture> texture) const override;
		void FlushTextureSlots() override;


		void SetSampler(const Ref<Texture>& texture, uint32_t slot) override;
		void SetPushConstantBlock(const PushConstantBlock& block) override;

		void SetSamplerCube(const Ref<Cubemap>& cubemap) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;

	private:
		void UpdateDescriptorSets();

	private:
		Ref<Shader> m_Shader;
		mutable std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffersByName;
		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffersByBinding;

		std::vector<Ref<Texture>> m_Textures;
		Ref<Cubemap> m_Cubemap;
		Ref<Texture> m_FallbackTexture;

		std::vector<PushConstantBlock> m_PushConstantBlocks;

		std::vector<VkDescriptorSet> m_DescriptorSets;

		ShaderReflectionData m_ReflectionData;

		bool m_IsDirty = false;
		bool m_IsCubemapSet = false;
		bool m_HasCubemapSampler = false;
	};

}
