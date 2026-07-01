#pragma once
#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& filePath);
		~OpenGLTexture();

		void Bind(uint32_t slot) const override;
		void Bind(void* commandBuffer) const override;

	private:
		uint32_t m_Texture;
	};

}