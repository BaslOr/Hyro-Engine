#include "pch.h"
#include "Hyro/Renderer/RenderingObjects/IndexBuffer.h"

#include "Hyro/Core/Core.h"
#include "Hyro/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Hyro {

	Ref<IndexBuffer> Hyro::IndexBuffer::Create(uint32_t size)
	{
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLIndexBuffer>();
			break;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanIndexBuffer>(size);
			break;
		}
	}

	Ref<IndexBuffer> Hyro::IndexBuffer::Create(const std::vector<uint32_t>& indices)
	{
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices);
			break;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanIndexBuffer>(indices);
			break;
		}
	}

}