#pragma once
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include "Hyro/Renderer/Shader.h"
#include "Hyro/Renderer/RenderPrimitives.h"
#include "Hyro/Renderer/GraphicsPipeline.h"

namespace Hyro {

	class AssetManager {
	public:
		static Ref<Texture> LoadTexture(const std::string& key, const std::string& path);
		static Ref<Texture> GetTexture(const std::string& key);
		static Ref<Texture> GetFallbackTexture();

		static Ref<Shader> LoadShader(const std::string& key, const DepthInfo& depthInfo, const std::string& vertexPath, const std::string& fragmentPath);
		static Ref<Shader> GetShader(const std::string& key);

		static Ref<Mesh> LoadMesh(const std::string& key, const std::string& path);
		static Ref<Mesh> GetMesh(const std::string& key);

		static Ref<Material> LoadMaterial(const std::string& key, const Ref<Shader>& shader);
		static Ref<Material> GetMaterial(const std::string& key);
		static Ref<Material> GetDefaultMaterial(const Ref<Shader>& shader);

	private:
		static inline std::unordered_map<std::string, Ref<Texture>> s_Textures;
		static inline std::unordered_map<std::string, Ref<Shader>> s_Shaders;
		static inline std::unordered_map<std::string, Ref<Mesh>> s_Meshes;
		static inline std::unordered_map<std::string, Ref<Material>> s_Materials;
	};

}
