#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace Hyro {

	class ModelLoader {
	public:
		inline ModelLoader() {
            Assimp::Importer importer;

            const aiScene* scene = importer.ReadFile(
                "",
                aiProcess_Triangulate |
                aiProcess_GenNormals
            );

            if (!scene || !scene->mRootNode)
            {
                std::cerr << "Assimp error: "
                    << importer.GetErrorString()
                    << "\n";
            }

            std::cout << "Assimp loaded successfully!\n";
            std::cout << "Meshes:      " << scene->mNumMeshes << "\n";
            std::cout << "Materials:   " << scene->mNumMaterials << "\n";
            std::cout << "Animations:  " << scene->mNumAnimations << "\n";

            if (scene->mRootNode)
            {
                std::cout << "Root node:   "
                    << scene->mRootNode->mName.C_Str()
                    << "\n";
            }
		}

	private:

	};

}
