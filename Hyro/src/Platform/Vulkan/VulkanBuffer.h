#pragma once
#include "Platform/Vulkan/VulkanBase.h"
#include "Hyro/Renderer/VertexBuffer.h"

namespace Hyro {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer();
		VulkanVertexBuffer(const std::vector<Vertex>& vertices);
		~VulkanVertexBuffer();

		void SetData(const std::vector<Vertex>& vertices) override;
		void Bind() const override {}
		void Bind(void* commandBuffer) const override;

		//Since the Vertex Layout fixed it is ok to put this here for the moment
		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription();

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;

	};

}
