#include "pch.h"
#include "Shader.h"

#include <iostream>
#include "Hyro/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hyro/Core/Core.h"

namespace Hyro {

	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPI::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected! ");
			return nullptr;
			break;
		case GraphicsAPI::OpenGL:
			return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
			break;
		case GraphicsAPI::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet! ");
			return nullptr;
			break;
		}
	}

}
