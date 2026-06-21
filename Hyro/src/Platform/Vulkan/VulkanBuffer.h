#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/VertexBuffer.h"
#include "Hyro/Renderer/IndexBuffer.h"
#include "Hyro/Renderer/UniformBuffer.h"

namespace Hyro {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer();
		VulkanVertexBuffer(const std::vector<Vertex>& vertices);
		~VulkanVertexBuffer();

		void SetData(const std::vector<Vertex>& vertices) override;
		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		//Since the Vertex Layout fixed it is ok to put this here for the moment
		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription();

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};



	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer();
		VulkanIndexBuffer(const std::vector<uint32_t>& indices);
		~VulkanIndexBuffer();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;
		void SetData(const std::vector<uint32_t>& indices) override;

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;

	};

	class VulkanUniformBuffer : public UniformBuffer {
	public:
		VulkanUniformBuffer();
		~VulkanUniformBuffer();

		void Bind() const override;
		void Bind(void* commandBuffer, void* pipelineLayout) const override;

		void SetData(const UniformBufferData& ubo) override;

		VkDescriptorSet GetDescriptorSet() const;

		static VkDescriptorSetLayout GetDescriptorSetLayout();

	private:
		std::vector<VkDescriptorSet> m_DescriptorSets;
		std::vector<VkBuffer> m_Buffers;
		std::vector<VkDeviceMemory> m_BufferMemories;
		std::vector<void*> m_MappedMemories;

		static inline VkDescriptorSetLayout s_DescriptorSetLayout;
	};

}
