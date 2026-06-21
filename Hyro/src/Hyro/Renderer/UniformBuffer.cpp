#include "pch.h"
#include "Hyro/Renderer/UniformBuffer.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/Vulkan/VulkanBuffer.h"

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
            HYRO_LOG_CORE_WARN("Unifrom Buffers are not implemented yet on OpenGL side");
            return nullptr;
            break;
        case GraphicsAPIType::Vulkan:
            return CreateRef<VulkanUniformBuffer>();
            break;
        }
    }

}
