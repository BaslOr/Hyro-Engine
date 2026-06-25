#include "pch.h"
#include "ResourceManager.h"


namespace Hyro {

	void ResourceManager::LoadTexture(const std::string& key, const std::string& path)
	{
		s_Textures[key] = Texture::Load(path);
	}

	Ref<Texture> ResourceManager::GetTexture(const std::string& key)
	{
		return s_Textures[key];
	}

}
