#pragma once
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/Texture.h"
#include "Hyro/Renderer/Shader.h"

namespace Hyro {

	class AssetManager {
	public:
		static void LoadTexture(const std::string& key, const std::string& path);
		static Ref<Texture> GetTexture(const std::string& key);

		static void LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath);
		static Ref<Shader> GetShader(const std::string& key);

	private:
		static inline std::unordered_map<std::string, Ref<Texture>> s_Textures;
		static inline std::unordered_map<std::string, Ref<Shader>> s_Shaders;
	};

}
