#pragma once
#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/GraphicsAPI.h"
#include "Hyro/Renderer/GraphicsPipeline.h"


namespace Hyro {

	class Renderer {
	public:
		struct Settings {
			GraphicsPipelineSettings PipelineSettings;
		};

	public:
		static void Init(const Settings& settings);
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		inline static GraphicsAPIType GetAPI() { return m_GraphicsAPIType; }

	private:
		static inline GraphicsAPIType m_GraphicsAPIType = GraphicsAPIType::OpenGL;
	};

}