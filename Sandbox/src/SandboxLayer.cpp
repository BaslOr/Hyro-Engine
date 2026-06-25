#include "SandboxLayer.h"
#include <imgui.h>


SandboxLayer::SandboxLayer()
{
	m_Texture = Hyro::Texture::Load("Assets/Textures/Cow.png");
}

void SandboxLayer::OnUpdate(const Hyro::TimeStep deltaTime)
{
	Hyro::Renderer::BeginScene();

	//For Test Purpose only
	Hyro::Renderer2D::DrawSprite(m_Texture, { 100.0f, 100.0f }, { 200.0f, 200.0f });

	Hyro::Renderer::EndScene();
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
