#include "pch.h"
#include "Hyro/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "Platform/Vulkan/VulkanAPI.h"

namespace Hyro {


	Scope<GraphicsAPI> GraphicsAPI::Create(GraphicsAPIType api)
	{
		switch (api)
		{
		case Hyro::GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No GraphicsAPI Selected");
		case Hyro::GraphicsAPIType::OpenGL:
			return CreateScope<OpenGLGraphicsAPI>();
			break;
		case Hyro::GraphicsAPIType::Vulkan:
			return CreateScope<VulkanAPI>();
			break;
		}
	}

}
