#include "pch.h"
#include "Hyro/Renderer/GraphicsContext.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"


namespace Hyro {

	Scope<GraphicsContext> GraphicsContext::Create(GraphicsAPIType type, void* windowHandle)
	{
        switch (Renderer::GetAPI())
        {
        case GraphicsAPIType::None:
            HYRO_LOG_CORE_FATAL("No Graphics API selected!");
            break;
        case GraphicsAPIType::OpenGL:
            return CreateScope<OpenGLContext>(windowHandle);
            break;
        case GraphicsAPIType::Vulkan:
            return CreateScope<VulkanContext>();
            break;
        }
	}

}