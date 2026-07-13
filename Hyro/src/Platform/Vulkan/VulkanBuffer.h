#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/VertexBuffer.h"
#include "Hyro/Renderer/IndexBuffer.h"
#include "Hyro/Renderer/UniformBuffer.h"

namespace Hyro {

	class VulkanBuffer {
	public:
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void CreateBufer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	};



	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(const VertexLayout& layout, uint32_t vertexCountHint);
		~VulkanVertexBuffer();

		void SetData(const std::vector<Vertex>& vertices) override;
		void Bind() const override;
		void Bind(void* commandBuffer) const override;
	
		inline VertexLayout GetLayout() const override { return m_Layout; }

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
		uint32_t m_Size;

		VertexLayout m_Layout;
	};





	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer(uint32_t size);
		VulkanIndexBuffer(const std::vector<uint32_t>& indices);
		~VulkanIndexBuffer();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;
		void SetData(const std::vector<uint32_t>& indices) override;

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
		uint32_t m_Size;
	};




	class VulkanUniformBuffer : public UniformBuffer {
	public:
		VulkanUniformBuffer();
		~VulkanUniformBuffer();

		void Bind() const override;
		void Bind(void* commandBuffer, void* pipelineLayout) const override;

		void SetData(const UniformBufferData& ubo) override;

		inline VkBuffer GetBufferAtIndex(size_t index) const { return m_Buffers[index]; }

	private:
		std::vector<VkBuffer> m_Buffers;
		std::vector<VkDeviceMemory> m_BufferMemories;
		std::vector<void*> m_MappedMemories;
	};

}
