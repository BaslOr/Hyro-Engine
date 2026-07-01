#include "pch.h"
#include "Hyro/Renderer/Material.h"

#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLMaterial.h"
//#include "Platform/Vulkan/VulkanMaterial.h"


namespace Hyro {

	Ref<Material> Material::Create(Ref<Shader> shader)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API is selected!");
			return nullptr;
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLMaterial>(shader);
		case GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet!");
			//return CreateRef<VulkanMaterial>(shader);
		}
	}

}
