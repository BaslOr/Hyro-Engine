#include "pch.h"
#include "Texture.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/Vulkan/VulkanTexture.h"


namespace Hyro {

    Ref<Texture> Texture::Load(const std::string & filePath)
    {
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected!");
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLTexture>(filePath);
			break;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanTexture>(filePath);
			break;
		}
    }

}
