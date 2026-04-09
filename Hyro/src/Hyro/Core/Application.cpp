#include "pch.h"
#include "Application.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "Hyro/Core/Logger.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Hyro {

	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
	{
		if (s_Instance) {
			HYRO_LOG_CORE_ERROR("Application already exits!");
			HYRO_ASSERT(false);
			return;
		}
		s_Instance = this;

		Logger::Init();

		WindowProps props(name, width, height);

		m_Window = WindowsWindow::Create();
		m_Window->SetEventCallback(HYRO_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {

	}

	void Application::Run() {
		m_LastTime = glfwGetTime(); //Move into Platform class

		while (m_IsRunning) {
			m_CurrentTime = glfwGetTime(); //Move into platfrom class
			TimeStep deltaTime = static_cast<float>(m_CurrentTime - m_LastTime);
			m_LastTime = m_CurrentTime;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			ImGuiIO& io = ImGui::GetIO();
			io.DeltaTime = deltaTime;
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate(deltaTime);
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>([this] (WindowCloseEvent& event) {
			m_IsRunning = false;
			return true;
			});

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}
}

