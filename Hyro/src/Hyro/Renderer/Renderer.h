#pragma once
#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsAPI.h"


namespace Hyro {

	class Renderer {
	public:
		struct Settings {
			bool enableBlendFunction = false;
			uint32_t SampleCount = 1;
		};

	public:
		static void Init(const Settings& settings);
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

		static void DrawIndexed(uint32_t count);
		static void Clear();
		static void SetClearColor(const glm::vec4& color);

	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::Vulkan;

		static inline Scope<GraphicsAPI> m_GraphicsAPI;
	};

}