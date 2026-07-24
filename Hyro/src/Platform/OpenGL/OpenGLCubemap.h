#pragma once
#include "Hyro/Renderer/Cubemap.h"

namespace Hyro {

	class OpenGLCubemap : public Cubemap {
	public:
		OpenGLCubemap(const std::string& filePath);
		~OpenGLCubemap();

		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t m_ID;
	};

}
