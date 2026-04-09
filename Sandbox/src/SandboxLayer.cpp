#include "SandboxLayer.h"
#include <imgui.h>


SandboxLayer::SandboxLayer(Hyro::Ref<Hyro::Window> window)
	: m_Renderer(Hyro::CreateRef<Hyro::Renderer2D>(window))
{

}

void SandboxLayer::OnUpdate(const Hyro::TimeStep deltaTime)
{
	m_Renderer->BeginScene();

	m_Renderer->DrawRect({ 100.0f, 100.0f }, { 200.0f, 200.0f }, { 0.1f, 0.1f, 0.8f, 1.0f });

	m_Renderer->EndScene();
}

void SandboxLayer::OnImGuiRender()
{
	static bool show = false;
	ImGui::ShowDemoWindow(&show);
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}
