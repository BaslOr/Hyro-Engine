#include "pch.h"
#include "Shader.h"

#include "Hyro/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Hyro {

	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
			break;
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet! ");
			return nullptr;
			break;
		}
	}

}
