#include "pch.h"
#include "VertexBuffer.h"
#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include "Hyro/Core/Core.h"

#include <iostream>

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
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet! ");
			return nullptr;
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
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet! ");
			return nullptr;
			break;
		}
    }

}