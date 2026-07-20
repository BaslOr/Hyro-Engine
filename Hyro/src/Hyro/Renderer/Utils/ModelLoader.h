#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <assimp/cimport.h>

namespace Hyro {

	class ModelLoader {
	public:
		inline static Ref<Mesh> LoadMesh(const std::string& modelPath, const std::string& texturePath) {
			const aiScene* scene = aiImportFile(modelPath.c_str(), aiProcess_Triangulate);
			auto mesh = scene->mMeshes[0];

			std::vector<Vertex3D> vertices;
			vertices.reserve(mesh->mNumVertices);
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				const aiVector3D v = mesh->mVertices[i];
				const aiColor4D c = mesh->mColors[0] ? mesh->mColors[0][i] : aiColor4D(1, 1, 1, 1);
				const aiVector3D t = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);
				vertices.push_back({ { v.x, v.y, v.z }, { t.x, t.y, t.z }, { c.r, c.g, c.b, c.a } });
				
			}

			std::vector<uint32_t> indices;
			indices.reserve(3 * mesh->mNumFaces);
			for (uint32_t i = 0; i != mesh->mNumFaces; i++)
			{
				for (uint32_t j = 0; j != 3; j++)
				{
					indices.push_back(mesh->mFaces[i].mIndices[j]);
				}
			}

			Ref<Texture> texture = Texture::Load(texturePath);

			Mesh output(vertices, indices, texture);
			return CreateRef<Mesh>(output);
		}

	private:

	};

}
