#pragma once
#include <array>

#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Project/AssetManager.h"

namespace Hyro {

	class MeshFactory {
    public:
		inline static Ref<Mesh> CreateCube()
		{
			auto vertices = GetCubeVertices();
			auto indices = GetCubeIndices();
            Ref<Texture> texture = AssetManager::GetTexture("Default");

            Mesh mesh(vertices, indices, texture);

			return CreateRef<Mesh>(mesh);
		}

    private:
        inline static std::vector<Vertex> GetCubeVertices()
        {
            return { {
				//         Position                           UV                        Color            SpriteIndex
                { glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // unten hinten links
                { glm::vec3(1.0f, -1.0f, -1.0f),   glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // unten hinten rechts
                { glm::vec3(1.0f,  1.0f, -1.0f),   glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // oben hinten rechts
                { glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // oben hinten links
                { glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // unten vorne links
                { glm::vec3(1.0f, -1.0f,  1.0f),   glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // unten vorne rechts
                { glm::vec3(1.0f,  1.0f,  1.0f),   glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f },  // oben vorne rechts
                { glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f }   // oben vorne links
            } };
        }

        inline static std::vector<uint32_t> GetCubeIndices()
        {
            return {
                // Front face
                4, 5, 6,
                6, 7, 4,

                // Back face
                0, 3, 2,
                2, 1, 0,

                // Left face
                0, 4, 7,
                7, 3, 0,

                // Right face
                1, 2, 6,
                6, 5, 1,

                // Top face
                3, 7, 6,
                6, 2, 3,

                // Bottom face
                0, 1, 5,
                5, 4, 0
            };
        }
	};

}
