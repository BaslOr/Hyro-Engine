#include "pch.h"
#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Hyro/Core/Core.h"

namespace Hyro {

	Ref<IndexBuffer> Hyro::IndexBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPI::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPI::OpenGL:
			return CreateRef<OpenGLIndexBuffer>();
			break;
		case GraphicsAPI::Vulkan:
			HYRO_LOG_CORE_FATAL("Vulkan is not supported yet! ");
			return nullptr;
			break;
		}
	}

	Ref<IndexBuffer> Hyro::IndexBuffer::Create(const std::vector<uint32_t>& indices)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPI::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPI::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices);
			break;
		case GraphicsAPI::Vulkan:
			HYRO_LOG_CORE_FATAL("Vulkan is not supported yet! ");
			return nullptr;
			break;
		}
	}

}