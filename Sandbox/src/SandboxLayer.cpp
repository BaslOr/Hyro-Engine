#include "SandboxLayer.h"
#include <imgui.h>


SandboxLayer::SandboxLayer()
{
	Hyro::ResourceManager::LoadTexture("Sample1", "Assets/Textures/Cow.png");
	Hyro::ResourceManager::LoadTexture("Sample2", "Assets/Textures/Texture.jpg");
}

void SandboxLayer::OnUpdate(const Hyro::TimeStep deltaTime)
{
	Hyro::Renderer::BeginScene();

	auto texture1 = Hyro::ResourceManager::GetTexture("Sample1");
	auto texture2 = Hyro::ResourceManager::GetTexture("Sample2");
	Hyro::Renderer2D::DrawSprite(texture1, { 100.0f, 100.0f }, { 200.0f, 200.0f });
	Hyro::Renderer2D::DrawSprite(texture2, { 400.0f, 100.0f }, { 200.0f, 200.0f });

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
