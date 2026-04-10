#include "pch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Hyro {
	void Renderer::Init(const Settings& settings)
	{
		InitGraphicsAPI();

		m_GraphicsAPI->SetBlendFunction(settings.enableBlendFunction);
		m_GraphicsAPI->SetSampleCount(settings.SampleCount);
		m_GraphicsAPI->SetUpDebugCallback();
		//Set Default Vertex Layout

		//Init Instance/Device On Vulkan, ...
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::DrawIndexed(uint32_t count)
	{
		m_GraphicsAPI->DrawIndexed(count);
	}

	void Renderer::Clear()
	{
		m_GraphicsAPI->Clear();
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		m_GraphicsAPI->SetClearColor(color);
	}

	void Renderer::InitGraphicsAPI()
	{
		switch (m_GraphicsAPIType)
		{
		case Hyro::GraphicsAPIType::None:
			HYRO_LOG_CORE_FATAL("No Graphics API selected!");
			break;
		case Hyro::GraphicsAPIType::OpenGL:
			m_GraphicsAPI = CreateScope<OpenGLGraphicsAPI>();
			break;
		case Hyro::GraphicsAPIType::Vulkan:
			HYRO_LOG_CORE_ERROR("Vulkan is not supported yet!");
			break;
		}
	}
}