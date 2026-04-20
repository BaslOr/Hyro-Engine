#pragma once
#include "Hyro/Renderer/VertexBuffer.h"

namespace Hyro::Vulkan {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer() = default;
		~VulkanVertexBuffer() = default;

		void SetData(const std::vector<Vertex>& data) override {}
		void Bind() const override {}
		void UnBind() const override {}

	};

}