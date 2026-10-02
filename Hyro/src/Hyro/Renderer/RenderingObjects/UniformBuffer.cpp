#include "pch.h"
#include "Hyro/Renderer/RenderingObjects/UniformBuffer.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Hyro {

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t binding, uint32_t size)
    {
        switch (SceneRenderer::GetAPI())
        {
        case GraphicsAPIType::None:
            HYRO_LOG_CORE_FATAL("No Graphics API selected!");
            return nullptr;
            break;
        case GraphicsAPIType::OpenGL:
            return CreateRef<OpenGLUniformBuffer>(binding, size);
            break;
        case GraphicsAPIType::Vulkan:
            return CreateRef<VulkanUniformBuffer>(size);
            break;
        }
    }

}
