#pragma once
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class ResourceManager {
	public:
		static void LoadTexture(const std::string& key, const std::string& path);
		static Ref<Texture> GetTexture(const std::string& key);

	private:
		static inline std::unordered_map<std::string, Ref<Texture>> s_Textures;
	};

}
