#include "pch.h"
#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	Ref<Texture> AssetManager::LoadTexture(const std::string& key, const std::string& path)
	{
		if (s_Textures.find(key) == s_Textures.end()) {
			s_Textures[key] = Texture::Load(path);
			return s_Textures[key];
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to laod Texture with allready existing key!");
			return nullptr;
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

	Ref<Shader> AssetManager::LoadShader(const std::string& key, const DepthInfo& depthInfo, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (s_Shaders.find(key) == s_Shaders.end()) {
			s_Shaders[key] = Shader::Create(depthInfo, vertexPath, fragmentPath);
			return s_Shaders[key];
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to laod Shader with allready existing key!");
			return nullptr;
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

	Ref<Mesh> AssetManager::LoadMesh(const std::string& key, const std::string& path)
	{
		if (s_Meshes.find(key) == s_Meshes.end()) {
			//s_Meshes[key] = Mesh::Load(path); TODO: Save Meshes to AssetManager
			return nullptr;
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to load Mesh with already existing key!");
			return nullptr;
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

	Ref<Material> Hyro::AssetManager::LoadMaterial(const std::string& key, const Ref<Shader>& shader) {
		if (s_Materials.find(key) == s_Materials.end()) {
			return s_Materials[key] = Material::Create(shader);
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to load Material with already existing key!");
			return nullptr;
		}
	}

	Ref<Material> Hyro::AssetManager::GetMaterial(const std::string& key)
	{
		if (s_Materials.find(key) != s_Materials.end()) {
			return s_Materials[key];
		}

		HYRO_LOG_CORE_ERROR("Failed to find Material with key: {0}!", key.c_str());
		return nullptr;
	}

	Ref<Material> Hyro::AssetManager::GetDefaultMaterial(const Ref<Shader>& shader)
	{
		if (s_Materials.find("Default" + shader->GetPath()) == s_Materials.end()) {
			auto material = Material::Create(shader);
			auto texture = GetTexture("Default");

			material->SetAlbedo(texture);
			material->SetAmbientOcclusion(texture);
			material->SetNormal(texture);
			material->SetRoughness(texture);
			
			s_Materials["Default" + shader->GetPath()] = std::move(material);
		}

		return s_Materials["Default" + shader->GetPath()];
	}

}