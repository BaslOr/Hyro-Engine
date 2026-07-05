#include "pch.h"
#include "Hyro/Renderer/UniformBuffer.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Hyro {

    Ref<UniformBuffer> UniformBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
        case GraphicsAPIType::None:
            HYRO_LOG_CORE_FATAL("No Graphics API selected!");
            return nullptr;
            break;
        case GraphicsAPIType::OpenGL:
            return CreateRef<OpenGLUniformBuffer>();
            break;
        case GraphicsAPIType::Vulkan:
            return CreateRef<VulkanUniformBuffer>();
            break;
        }
    }

}
