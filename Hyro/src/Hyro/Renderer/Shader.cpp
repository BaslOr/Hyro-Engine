#include "pch.h"
#include "Shader.h"

#include "Hyro/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"


namespace Hyro {

	Ref<Shader> Shader::Create(const GraphicsPipelineSettings& settings)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected!");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLShader>(settings.VertexShaderPath, settings.FragmentShaderPath);
			break;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanShader>(settings);
			break;
		}
	}

}
