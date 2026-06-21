#pragma once
#include "Hyro/Renderer/VertexArray.h"

namespace Hyro {

	//Acts as a container for Vertex and Index Buffer to simplify RenderCommand::Submit() and API agnostic renderering
	class VulkanVertexArray : public VertexArray {
	public:
		VulkanVertexArray();
		~VulkanVertexArray() = default;

		void AddVertexBuffer(Ref<VertexBuffer> buffer) override;
		void SetIndexBuffer(Ref<IndexBuffer> buffer) override;

		void SetLayout(const VertexLayout& layout) override {}

		void Bind() const override {}
		void Bind(void* commandBuwffer) const override;

	private:
		std::vector<Ref<VertexBuffer>> m_VerexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	};

}