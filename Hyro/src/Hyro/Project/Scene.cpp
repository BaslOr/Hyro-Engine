#include "pch.h"
#include "Hyro/Project/Scene.h"


#include "Hyro/Renderer/Renderer2D.h"
#include "Hyro/Renderer/Renderer.h"


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
		SceneRenderer::BeginRenderPass();

		Renderer2D::BeginScene();
        for (auto& spriteInstance : m_Sprites)
        {
			glm::vec2 position = glm::vec2(spriteInstance.Transform[3]);
			glm::vec2 size = glm::vec2(spriteInstance.Transform[0][0], spriteInstance.Transform[1][1]);
			Renderer2D::DrawSprite(spriteInstance.Sprite, position, size);
        }
        Renderer2D::EndScene();


        SceneRenderer::BeginScene();
		for (auto& meshInstance : m_Meshes)
		{
			SceneRenderer::DrawMesh(meshInstance.Mesh, meshInstance.Transform);
		}
		SceneRenderer::EndScene();

        SceneRenderer::EndRenderPass();
    }




}
