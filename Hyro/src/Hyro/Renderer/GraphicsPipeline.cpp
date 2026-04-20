#include "pch.h"
#include "Hyro/Renderer/GraphicsPipeline.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/OpenGL/OpenGLGraphicsPipeline.h"

namespace Hyro {

	Ref<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsPipelineSettings& settings)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPIType::None:
				HYRO_LOG_CORE_ERROR("Graphics API None is not supported!");
				return nullptr;
			case GraphicsAPIType::OpenGL:
				return CreateRef<OpenGLGraphicsPipeline>(settings);
			case GraphicsAPIType::Vulkan:
				return CreateRef<VulkanGraphicsPipeline>(settings);
		}
	}

}