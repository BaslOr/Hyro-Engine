#pragma once
#include "Hyro/Renderer/GraphicsContext.h"

namespace Hyro {


	class Renderer {
	public:
		inline static GraphicsAPI GetAPI() { return m_CurrentGraphicsAPI; }

	private:
		inline static GraphicsAPI m_CurrentGraphicsAPI = GraphicsAPI::OpenGL;
	};

}