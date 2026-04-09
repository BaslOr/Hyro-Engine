#pragma once
#include <string>
#include <stdint.h>

#include "Hyro/Events/Event.h"
#include "Hyro/Time/TimeStep.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	struct WindowProps {
		std::string Title;
		uint32_t Width, Height;
		
		WindowProps(const std::string& title = "New Window", uint32_t width = 1280, uint32_t height = 720)
			: Title(title), Width(width), Height(height) 
		{ }

	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() { }

		virtual void OnUpdate(TimeStep deltaTime) = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNative() const = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};

}
