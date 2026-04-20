#pragma once
#include "Hyro/Renderer/IndexBuffer.h"

namespace Hyro::Vulkan {

	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer() = default;
		~VulkanIndexBuffer() = default;


		void Bind() const override {}
		void UnBind() const override {}
		void SetData(const std::vector<uint32_t>& data) override {}

	};

}