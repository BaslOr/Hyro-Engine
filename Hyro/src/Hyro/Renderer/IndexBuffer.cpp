#include "pch.h"
#include "Hyro/Renderer/IndexBuffer.h"

#include "Hyro/Core/Core.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

namespace Hyro {

	Ref<IndexBuffer> Hyro::IndexBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLIndexBuffer>();
			break;
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_FATAL("Vulkan Index buffer are not implemented yet");
			return CreateRef<Vulkan::VulkanIndexBuffer>();
			break;
		}
	}

	Ref<IndexBuffer> Hyro::IndexBuffer::Create(const std::vector<uint32_t>& indices)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices);
			break;
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_FATAL("Vulkan Index buffer are not implemented yet");
			return CreateRef<Vulkan::VulkanIndexBuffer>();
			break;
		}
	}

}