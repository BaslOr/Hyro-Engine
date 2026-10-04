#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <assimp/cimport.h>

namespace Hyro {

	class ModelLoader {
	public:
		inline static Ref<Mesh> LoadMesh(const std::string& modelPath) {
			const aiScene* scene = aiImportFile(modelPath.c_str(), aiProcess_Triangulate);
			auto mesh = scene->mMeshes[0];

			std::vector<Vertex3D> vertices;
			vertices.reserve(mesh->mNumVertices);
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				const aiVector3D v = mesh->mVertices[i];
				const aiVector3D t = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);
				const aiVector3D n = mesh->mNormals ? mesh->mNormals[i] : aiVector3D(0, 0, 1);
				vertices.push_back({ { v.x, v.y, v.z }, { t.x, t.y, t.z }, { n.x, n.y, n.z } });
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

			//Ref<Texture> albedo = Texture::Load(albedoPath);
			//Ref<Texture> normal = Texture::Load(normalMapPath);
			//Ref<Texture> ao = Texture::Load(ambientOcclusionPath);
			//Ref<Texture> roughness = Texture::Load(roughnessPath);

			Mesh output(vertices, indices);
			aiReleaseImport(scene);
			return CreateRef<Mesh>(output);
		}

	private:

	};

}
