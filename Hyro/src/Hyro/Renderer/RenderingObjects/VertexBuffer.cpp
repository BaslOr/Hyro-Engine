#include "pch.h"
#include "Hyro/Renderer/RenderingObjects/VertexBuffer.h"
#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

#include "Hyro/Core/Core.h"

namespace Hyro {

	Ref<VertexBuffer> VertexBuffer::Create(const VertexLayout& layout, uint32_t vertexCountHint)
	{
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(layout, vertexCountHint);
			break;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanVertexBuffer>(layout, vertexCountHint);
			break;
		}
	}

}