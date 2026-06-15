#include "pch.h"
#include "VulkanVertexArray.h"

namespace Hyro {

	void VulkanVertexArray::AddVertexBuffer(Ref<VertexBuffer> buffer)
	{
		m_VerexBuffers.push_back(buffer);
	}

	void VulkanVertexArray::SetIndexBuffer(Ref<IndexBuffer> buffer)
	{
		m_IndexBuffer = buffer;
	}

	void VulkanVertexArray::Bind(void* commandBuffer) const
	{
		for (const auto vertexBuffer : m_VerexBuffers)
			vertexBuffer->Bind(commandBuffer);
		m_IndexBuffer->Bind(commandBuffer);
	}

}
