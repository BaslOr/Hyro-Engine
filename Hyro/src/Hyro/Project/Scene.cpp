#include "pch.h"
#include "Hyro/Project/Scene.h"


#include "Hyro/Core/Application.h"

#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Renderer/Renderer3D.h"
#include "Hyro/Renderer/Renderer.h"
#include "Hyro/Renderer/MeshFactory.h"


namespace Hyro {

    Ref<Scene> Scene::Create()
    {
        return CreateRef<Scene>();
    }

    MeshHandle Scene::AddMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
		m_Meshes.emplace_back(mesh, transform);
		return static_cast<MeshHandle>(m_Meshes.size() - 1);
    }

    MeshHandle Hyro::Scene::AddCube(const glm::mat4& transform)
    {
		return AddMesh(MeshFactory::CreateCube(), transform);
    }

    void Scene::SetMeshTransform(MeshHandle handle, const glm::mat4& transform)
    {
		m_Meshes[handle].Transform = transform;
    }

    SpriteHandle Scene::AddSprite(const Ref<Sprite>& sprite, const glm::mat4& transform)
    {
        m_Sprites.emplace_back(sprite, transform);
        return static_cast<SpriteHandle>(m_Sprites.size() - 1);
    }

    void Scene::SetSpriteTransform(SpriteHandle handle, const glm::mat4& transform)
    {
        m_Sprites[handle].Transform = transform;
    }

    void Scene::Render()
    {

		Renderer::BeginRenderPass();

        //Get framebuffer size
        m_Camera.Update(); //Of course update should not be here, but the focus for now is on rendering not game engine stuff
        Ref <Window> window = Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight()); 
        float aspectRatio = width / height;
        glm::mat4 projection;
        if (Renderer::GetAPI() == GraphicsAPIType::Vulkan)
            projection = glm::perspectiveRH_ZO(80.f, aspectRatio, 0.1f, 100.f);
        else if (Renderer::GetAPI() == GraphicsAPIType::OpenGL)
            projection = glm::perspectiveRH_NO(80.f, aspectRatio, 0.1f, 100.f);
        
        glm::mat4 view = m_Camera.GetViewMatrix();
        glm::mat4 viewProjection = projection * view;

        Renderer3D::BeginScene(viewProjection);
        for (auto& meshInstance : m_Meshes)
        {
            Renderer3D::DrawMesh(meshInstance.Mesh, meshInstance.Transform);
        }
        Renderer3D::EndScene();


        if (Renderer::GetAPI() == GraphicsAPIType::Vulkan)
            projection = glm::ortho(0.f, width, height, 0.f, -100.0f, 100.f);
        else if (Renderer::GetAPI() == GraphicsAPIType::OpenGL)
            projection = glm::ortho(0.f, width, 0.f, height, -100.0f, 100.f);

        viewProjection = projection * view;

		Renderer2D::BeginScene(projection);
        for (auto& spriteInstance : m_Sprites)
        {
            glm::vec2 position = glm::vec2(spriteInstance.Transform[3]);
			glm::vec2 size = glm::vec2(spriteInstance.Transform[0][0], spriteInstance.Transform[1][1]);
			Renderer2D::DrawSprite(spriteInstance.Sprite, position, size);
        }
        Renderer2D::EndScene();

        Renderer::EndRenderPass();
    }

}
