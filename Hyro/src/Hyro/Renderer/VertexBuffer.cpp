#include "pch.h"
#include "Hyro/Renderer/VertexBuffer.h"
#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

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
			return CreateRef<VulkanVertexBuffer>();
			break;
		}
	}

	Ref<VertexBuffer> Hyro::VertexBuffer::Create(const std::vector<Vertex>& vertices)
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
			return CreateRef<VulkanVertexBuffer>(vertices);
			break;
		}
    }

}