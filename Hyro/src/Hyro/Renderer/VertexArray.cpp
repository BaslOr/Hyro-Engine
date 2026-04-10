#include "pch.h"
#include "VertexArray.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hyro {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected!");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLVertexArray>();
			break;
		case GraphicsAPIType::Vulkan:
			//Vulkan has no equivalent to vertex arrays
			break;
		}
	}

}
