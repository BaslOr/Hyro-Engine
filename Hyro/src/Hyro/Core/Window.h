#pragma once

#include "Hyro/Events/Event.h"
#include "Hyro/Time/TimeStep.h"
#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/GraphicsContext.h"

namespace Hyro {

	struct WindowProps {
		std::string Title;
		uint16_t Width, Height;
		uint32_t SampleCount;
		
		WindowProps(const std::string& title = "New Window", uint16_t width = 1280, uint16_t height = 720, uint32_t sampleCount = 1)
			: Title(title), Width(width), Height(height), SampleCount(sampleCount)
		{ }

	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		static Ref<Window> Create(const WindowProps& props = WindowProps());

		virtual ~Window() { }

		virtual void OnUpdate(TimeStep deltaTime) = 0;

		virtual uint16_t GetWidth() = 0;
		virtual uint16_t GetHeight() = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNative() const = 0;
	};

}
