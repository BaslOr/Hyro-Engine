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

	Ref<Texture> Hyro::AssetManager::GetFallbackTexture()
	{
		if (s_Textures.find("FALLBACK_TEXTURE") != s_Textures.end()) {
			return s_Textures["FALLBACK_TEXTURE"];
		}

		s_Textures["FALLBACK_TEXTURE"] = Texture::Load("Assets/Textures/Fallback.png");
		return s_Textures["FALLBACK_TEXTURE"];
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

	void Hyro::AssetManager::LoadMesh(const std::string& key, const std::string& path)
	{
		if (s_Meshes.find(key) == s_Meshes.end()) {
			//s_Meshes[key] = Mesh::Load(path); TODO Implement ModelLoader
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to load Mesh with already existing key!");
		}
	}

	Ref<Mesh> Hyro::AssetManager::GetMesh(const std::string& key)
	{
		if (s_Meshes.find(key) != s_Meshes.end()) {
			return s_Meshes[key];
		}

		HYRO_LOG_CORE_ERROR("Failed to find Mesh with key: {0}!", key.c_str());
		return nullptr;
	}

}
