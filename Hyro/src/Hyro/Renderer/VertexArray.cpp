#include "pch.h"
#include "VertexArray.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"

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
			return CreateRef<VulkanVertexArray>();//Acts as a wapper for vertexBuffer + indexBuffers
			break;
		}
	}

}
