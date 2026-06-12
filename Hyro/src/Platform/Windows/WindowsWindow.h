#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Hyro/Core/Window.h"
#include "Hyro/Time/TimeStep.h"
#include "Hyro/Core/Memory.h"
#include <Hyro/Renderer/GraphicsContext.h>

namespace Hyro {

	class WindowsWindow : public Window {
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate(TimeStep deltaTime) override;

		inline uint16_t GetWidth() override { return m_Data.Width; }
		inline uint16_t GetHeight() override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return m_Data.VSync; }

		inline void* GetNative() const override { return (void*)m_Window; }

	private:
		void SetupWindowHints();

		void SetupGLFWCallbacks();

	private:
		GLFWwindow* m_Window;


		struct WindowData
		{
			std::string Title;
			uint16_t Width, Height;
			bool VSync;

			Ref<GraphicsContext> GraphicsContext;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}
