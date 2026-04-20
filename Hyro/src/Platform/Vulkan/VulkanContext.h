#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

namespace Hyro {

	class VulkanContext : public GraphicsContext{
	public:
		VulkanContext() = default;
		~VulkanContext() = default;

		void Init() override {}
		void SwapBuffers() const override {}
		void ResizeViewport(uint32_t width, uint32_t height) override {}

	};

}