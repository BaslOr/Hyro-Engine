#include "pch.h"
#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	void AssetManager::LoadTexture(const std::string& key, const std::string& path)
	{
		if (s_Textures.find(key) == s_Textures.end()) {
			s_Textures[key] = Texture::Load(path);
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to laod Texture with allready existing key!");
		}
	}

	Ref<Texture> AssetManager::GetTexture(const std::string& key)
	{
		if (s_Textures.find(key) != s_Textures.end()) {
			return s_Textures[key];
		}

		HYRO_LOG_CORE_ERROR("Failed to find Texture with key: {0}!", key.c_str());
		return nullptr;
	}

	void AssetManager::LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (s_Shaders.find(key) == s_Shaders.end()) {
			s_Shaders[key] = Shader::Create(vertexPath, fragmentPath);
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to laod Shader with allready existing key!");
		}
	}

	Ref<Shader> AssetManager::GetShader(const std::string& key)
	{
		if (s_Shaders.find(key) != s_Shaders.end()) {
			return s_Shaders[key];
		}

		HYRO_LOG_CORE_ERROR("Failed to find Shader with key: {0}!", key.c_str());
		return nullptr;
	}

}
