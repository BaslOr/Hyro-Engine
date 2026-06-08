#include "pch.h"
#include "Hyro/Renderer/GraphicsBackend.h"

#include "Platform/OpenGL/OpenGLBackend.h"
#include "Platform/Vulkan/VulkanBackend.h"


namespace Hyro {

	Scope<GraphicsBackend> GraphicsBackend::Create(GraphicsAPIType type, void* windowHandle)
	{
        switch (Renderer::GetAPI())
        {
        case GraphicsAPIType::None:
            HYRO_LOG_CORE_FATAL("No Graphics API selected!");
            break;
        case GraphicsAPIType::OpenGL:
            return CreateScope<OpenGLBackend>(windowHandle);
            break;
        case GraphicsAPIType::Vulkan:
            return CreateScope<VulkanBackend>();
            break;
        }
	}

}