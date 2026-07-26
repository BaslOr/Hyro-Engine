#pragma once
#include "Hyro/Renderer/Material.h"

#include "Platform/Vulkan/VulkanBase.h"


namespace Hyro {

	class VulkanMaterial : public Material {
	public:
		VulkanMaterial(Ref<Shader> shader);

		void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer) override;

		void SetTextures(const std::array<Ref<Texture>, 16>& textures) override;
		void SetTexture(const Ref<Texture>& texture, uint32_t slot) override { }
		void SetPushConstantBlock(const PushConstantBlock& block) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;

	private:
		void UpdateDescriptorSets();

	private:
		Ref<Shader> m_Shader;
		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::array<Ref<Texture>, 16> m_Textures;

		Ref<Texture> m_FallbackTexture;

		std::vector<PushConstantBlock> m_PushConstantBlocks;

		std::vector<VkDescriptorSet> m_DescriptorSets;

		bool m_IsDirty = false;
	};

}
