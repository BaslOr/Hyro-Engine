#include "SandboxLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>


SandboxLayer::SandboxLayer()
{
	Hyro::AssetManager::LoadTexture("Sample1", "Assets/Textures/Cow.png");
	Hyro::AssetManager::LoadTexture("Sample2", "Assets/Textures/Texture.jpg");

	m_Scene = Hyro::Scene::Create();

	//Prepare scene
	//Hyro::Ref<Hyro::Sprite> sprite1 = Hyro::CreateRef<Hyro::Sprite>();
	//sprite1->Sprite = Hyro::AssetManager::GetTexture("Sample1");
	//Hyro::Ref<Hyro::Sprite> sprite2 = Hyro::CreateRef<Hyro::Sprite>();
	//sprite2->Sprite = Hyro::AssetManager::GetTexture("Sample2");
	//glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 1.0f));
	//glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 100.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 1.0f));

	//m_Scene->AddSprite(sprite1, transform1);
	//m_Scene->AddSprite(sprite2, transform2);
	
	glm::mat4 meshTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	meshTransform = glm::rotate(meshTransform, 90.f, glm::vec3(1.0f, 0.0f, 0.0f));
	Hyro::Ref<Hyro::Mesh> mesh = Hyro::ModelLoader::LoadMesh("Assets/Models/DamagedHelmet/glTF/DamagedHelmet.gltf", "Assets/Models/DamagedHelmet/glTF/Default_albedo.jpg");
	m_Scene->AddMesh(mesh, meshTransform);
}

void SandboxLayer::OnUpdate(const Hyro::TimeStep deltaTime)
{
	m_Scene->Render();
}

void SandboxLayer::OnImGuiRender()
{
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}
