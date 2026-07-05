#pragma once
#include "Hyro/Renderer/RenderPrimitives.h"

#include <vector>

namespace Hyro {

	using MeshHandle = uint32_t;
	using SpriteHandle = uint32_t;

	struct MeshInstance {
		Ref<Mesh> Mesh;
		glm::mat4 Transform;
	};

	struct SpriteInstance {
		Ref<Sprite> Sprite;
		glm::mat4 Transform;
		glm::vec4 Color;
	};


	class Scene {
	public:
		static Ref<Scene> Create();

		MeshHandle AddMesh(const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
		MeshHandle AddCube(const glm::mat4& transform = glm::mat4(1.0f));
		void SetMeshTransform(MeshHandle handle, const glm::mat4& transform);


		SpriteHandle AddSprite(const Ref<Sprite>& sprite, const glm::mat4& transform = glm::mat4(1.0f));
		void SetSpriteTransform(SpriteHandle handle, const glm::mat4& transform);

		void Render();

		Scene() = default;
		~Scene() = default;
	private:

	private:
		std::vector<MeshInstance> m_Meshes;
		std::vector<SpriteInstance> m_Sprites;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};


}
