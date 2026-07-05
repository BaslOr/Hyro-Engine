#include "pch.h"
#include "Framebuffer.h"

#include "Hyro/Renderer/Renderer.h"

namespace Hyro {

	//It is questionable if this is even required to be implemented on global level
    Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height)
    {
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			HYRO_LOG_CORE_ERROR("OpenGL Framebuffers are not supported yet ");
			//return CreateRef<OpenGLFramebuffer>();
			break;
		case GraphicsAPIType::Vulkan:
			//return CreateRef<VulkanFramebuffer>();
			break;
		}
    }

}
