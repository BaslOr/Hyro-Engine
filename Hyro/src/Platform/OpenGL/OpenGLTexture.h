#pragma once
#include "Hyro/Renderer/Texture.h"

namespace Hyro {

	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& filePath);
		~OpenGLTexture();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		inline uint32_t GetSpriteIndex() const { return m_SpriteIndex; }

	private:
		uint32_t m_Texture;
		uint32_t m_SpriteIndex;
		static inline uint32_t s_NextSpriteIndex = 0;
	};

}