#include "pch.h"
#include "Hyro/Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"


namespace Hyro
{

	Ref<Window> Window::Create(const WindowProps& props)
	{
		switch (g_CurrentPlatform)
		{
			case PlatformType::Windows:
				return CreateRef<WindowsWindow>(props);
			case PlatformType::Linux:
				HYRO_LOG_CORE_FATAL("Linux isn't supported yet");
				return nullptr;
			case PlatformType::MacOS: 
				HYRO_LOG_CORE_FATAL("MacOS isn't supported yet");
				return nullptr;
		}
	}

}