#pragma once
#define GLFW_INCLUDE_NONE //Highly temporary

#include "Hyro/Events/Event.h"

#include "Hyro/Core/LayerStack.h"
#include "Hyro/Core/Window.h"

#include "Hyro/ImGui/ImGuiLayer.h"

namespace Hyro {

	class Application {
	public:
		Application(const std::string& name, uint32_t width, uint32_t height);
		~Application();

		inline static Application& Get() { return *s_Instance; }

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Ref<Window> GetWindow() const { return m_Window; }

	private:
		inline static Application* s_Instance = nullptr;
	private:
		Ref<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		double m_LastTime = 0.f;
		double m_CurrentTime = 0.f;

		bool m_IsRunning = true;
	};

}
