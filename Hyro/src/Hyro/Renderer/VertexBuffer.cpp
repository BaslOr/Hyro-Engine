#include "pch.h"
#include "Hyro/Renderer/VertexBuffer.h"
#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"

#include "Hyro/Core/Core.h"

namespace Hyro {

	Ref<VertexBuffer> VertexBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLVertexBuffer>();
			break;
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan Vertex Buffers are not supported yet ");
			return CreateRef<Vulkan::VulkanVertexBuffer>();
			break;
		}
	}

	Ref<VertexBuffer> Hyro::VertexBuffer::Create(std::vector<Vertex> vertices)
    {
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices);
			break;
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan Vertex Buffers are not supported yet ");
			return CreateRef<Vulkan::VulkanVertexBuffer>();
			break;
		}
    }

}