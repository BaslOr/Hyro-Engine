#pragma once
#include "Hyro/Renderer/GraphicsAPI.h"


namespace Hyro {

	enum class GraphicsAPIType{
		None = 0,
		OpenGL = 1,
		Vulkan = 2
	};

	class Renderer {
	public:
		struct Settings {
			bool enableBlendFunction = false;
			uint32_t SampleCount = 1;
		};

	public:
		static void Init(const Settings& settings);
		static void Shutdown();

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

		static void DrawIndexed(uint32_t count);
		static void Clear();
		static void SetClearColor(const glm::vec4& color);
		

	private:
		static void InitGraphicsAPI();

	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::OpenGL;

		static inline Scope<GraphicsAPI> m_GraphicsAPI;
	};

}