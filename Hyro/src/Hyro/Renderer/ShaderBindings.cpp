#include "pch.h"
#include "Hyro/Renderer/ShaderBindings.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShaderBindings.h"
#include "Platform/Vulkan/VulkanShaderBindings.h"


namespace Hyro {

	Ref<ShaderBindings> ShaderBindings::Create(Ref<Shader> shader)
	{
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API is selected!");
			return nullptr;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLShaderBindings>(shader);
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanShaderBindings>(shader);
		}
	}

}
