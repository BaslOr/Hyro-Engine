#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "Hyro/Events/Event.h"
#include "Hyro/Core/Window.h"
#include "Hyro/Time/TimeStep.h"
#include "Hyro/Core/Memory.h"
#include <Hyro/Renderer/GraphicsContext.h>

namespace Hyro {

	class WindowsWindow : public Window {
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		virtual void OnUpdate(TimeStep deltaTime) override;

		virtual inline uint32_t GetWidth() override { return m_Data.Width; }
		virtual inline uint32_t GetHeight() override { return m_Data.Height; }

		virtual inline void* GetNative() const override { return (void*)m_Window; }

		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		inline virtual bool IsVSync() const { return m_Data.VSync; }

	private:
		GLFWwindow* m_Window;


		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
			Ref<GraphicsContext> RendereringContext;
		};

		WindowData m_Data;
	};

}
