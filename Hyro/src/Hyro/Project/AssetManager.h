#pragma once
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/RenderPrimitives.h"

namespace Hyro {

	class AssetManager {
	public:
		static void LoadTexture(const std::string& key, const std::string& path);
		static Ref<Texture> GetTexture(const std::string& key);
		static Ref<Texture> GetFallbackTexture();

		static void LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath);
		static Ref<Shader> GetShader(const std::string& key);

		static void LoadMesh(const std::string& key, const std::string& path);
		static Ref<Mesh> GetMesh(const std::string& key);

	private:
		static inline std::unordered_map<std::string, Ref<Texture>> s_Textures;
		static inline std::unordered_map<std::string, Ref<Shader>> s_Shaders;
		static inline std::unordered_map<std::string, Ref<Mesh>> s_Meshes;
	};

}
