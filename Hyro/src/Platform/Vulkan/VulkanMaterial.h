#pragma once
#include "Hyro/Renderer/Material.h"

#include "Platform/Vulkan/VulkanBase.h"


namespace Hyro {

	class VulkanMaterial : public Material {
	public:
		VulkanMaterial(Ref<Shader> shader);

		void SetUnifromBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding) override;
		void SetTexture(Ref<Texture> texture, uint32_t slot) override;

		void SetPushConstants(const PushConstants& pushConstants) override;

		void Bind() override;
		void Bind(void* commandBuffer) override;

	private:
		void UpdateDescriptorSets();

	private:
		Ref<Shader> m_Shader;
		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::unordered_map<uint32_t, Ref<Texture>> m_Textures;

		PushConstants m_PushConstants;

		std::vector<VkDescriptorSet> m_DescriptorSets;

		bool m_IsDirty = false;
	};

}
