#pragma once
#include "Hyro/Renderer/RenderingObjects/Texture.h"
#include <cstdint>

namespace Hyro {

	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(uint32_t glID);
		OpenGLTexture(const std::string& filePath);
		~OpenGLTexture();

		void Bind(uint32_t slot) const override;
		void Bind(void* commandBuffer) const override;

	private:
		uint32_t m_Texture;
	};

}