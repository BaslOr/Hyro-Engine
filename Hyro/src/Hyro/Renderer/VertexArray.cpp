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



	uint32_t VertexLayout::GetVertexAttributeSize(VertexAttributeType type) const
	{
		switch (type)
		{
		case Hyro::VertexAttributeType::FLOAT:
			return 4;
			break;
		case Hyro::VertexAttributeType::FLOAT2:
			return 8;
			break;
		case Hyro::VertexAttributeType::FLOAT3:
			return 12;
			break;
		case Hyro::VertexAttributeType::FLOAT4:
			return 16;
			break;
		}
	}

}
