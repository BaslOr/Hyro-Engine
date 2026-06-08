#pragma once
#include "Hyro/Renderer/GraphicsBackend.h"

namespace Hyro {

	class VulkanBackend : public GraphicsBackend{
	public:
		VulkanBackend() = default;
		~VulkanBackend() = default;

		void Init() override {}
		void SwapBuffers() const override {}
		void ResizeViewport(uint32_t width, uint32_t height) override {}

		void GetCommandBuffer() {}

	};

}