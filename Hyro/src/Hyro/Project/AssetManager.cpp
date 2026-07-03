#include "pch.h"
#include "Hyro/Project/AssetManager.h"


namespace Hyro {

	void AssetManager::LoadTexture(const std::string& key, const std::string& path)
	{
		if (s_Textures.find(key) == s_Textures.end()) {
			s_Textures[key] = Texture::Load(path);
		}
		else {
			HYRO_LOG_CORE_ERROR("Tried to laod Resource with allready existing key!");
		}
	}

	Ref<Texture> AssetManager::GetTexture(const std::string& key)
	{
		if (s_Textures.find(key) != s_Textures.end()) {
			return s_Textures[key];
		}

		HYRO_LOG_CORE_ERROR("Failed to find Resource with key: {0}!", key.c_str());
		return nullptr;
	}

}
