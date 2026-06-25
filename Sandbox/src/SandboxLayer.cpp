#include "SandboxLayer.h"
#include <imgui.h>


SandboxLayer::SandboxLayer()
{
	Hyro::ResourceManager::LoadTexture("Sample", "Assets/Textures/Cow.png");
}

void SandboxLayer::OnUpdate(const Hyro::TimeStep deltaTime)
{
	Hyro::Renderer::BeginScene();

	//For Test Purpose only
	auto texture = Hyro::ResourceManager::GetTexture("Sample");
	Hyro::Renderer2D::DrawSprite(texture, { 100.0f, 100.0f }, { 200.0f, 200.0f });

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
