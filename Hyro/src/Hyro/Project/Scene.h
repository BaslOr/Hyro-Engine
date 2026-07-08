#pragma once
#include "Hyro/Renderer/RenderPrimitives.h"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include "Hyro/Core/Input.h"
#include <iostream>

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


	class SceneCamera {
	public:
		SceneCamera()
			: m_Position(0.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f), m_Front(0.f, 0.f, -1.0f), m_Speed(0.2f), m_Sensitivity(0.1f),
			m_LastMouseX(Input::GetMouseX()), m_LastMouseY(Input::GetMouseY()), m_Yaw(-90.0f), m_Pitch(0.0)
		{

		}
		~SceneCamera() = default;

		inline void Update() {
			if (Input::IsKeyPressed(Key::W)) m_Position           += m_Front * m_Speed;
			if (Input::IsKeyPressed(Key::S)) m_Position           -= m_Front * m_Speed;
			if (Input::IsKeyPressed(Key::A)) m_Position           += glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed;
			if (Input::IsKeyPressed(Key::D)) m_Position           -= glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed;
			if (Input::IsKeyPressed(Key::Space)) m_Position.y     += m_Speed;
			if (Input::IsKeyPressed(Key::LeftShift)) m_Position.y -= m_Speed;

			float deltaMouseX = m_LastMouseX - Input::GetMouseX();
			float deltaMouseY = m_LastMouseY - Input::GetMouseY();
			m_LastMouseX = Input::GetMouseX();
			m_LastMouseY = Input::GetMouseY();
			deltaMouseX *= m_Sensitivity;
			deltaMouseY *= m_Sensitivity;

			if (!Input::IsMouseButtonPressed(Mouse::ButtonRight))
				return;

			m_Yaw += deltaMouseX;
			m_Pitch += deltaMouseY;
			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			
			glm::vec3 direction;
			direction.x = glm::cos(glm::radians(m_Yaw));
			direction.y = glm::sin(glm::radians(m_Pitch));
			direction.z = glm::sin(glm::radians(m_Yaw));
			m_Front = glm::normalize(direction);
		}

		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Up;
		glm::vec3 m_Front;

		float m_LastMouseX, m_LastMouseY;
		float m_Yaw, m_Pitch;

		float m_Speed, m_Sensitivity;
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
		SceneCamera m_Camera;
	};


}
