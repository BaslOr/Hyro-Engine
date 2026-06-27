#include "pch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace Hyro {

	static uint32_t NumChannelsToOpenGLFormat(int nrChannels) {
		if (nrChannels == 4)
			return GL_RGBA;
		else
			return GL_RGB;
	}

	OpenGLTexture::OpenGLTexture(const std::string& filePath)
	{
		m_SpriteIndex = s_NextSpriteIndex;
		s_NextSpriteIndex++;

		int width, height, nrChannels;
		const char* path = filePath.c_str();
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path, &width, &height, &nrChannels, 4);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			HYRO_LOG_CORE_ERROR("Failed to load texture! Path: {0}", filePath.c_str());
		}

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void OpenGLTexture::Bind() const
	{
		glBindTextureUnit(m_SpriteIndex, m_Texture);
	}

	void OpenGLTexture::Bind(void* commandBuffer) const
	{
		HYRO_LOG_CORE_WARN("Tried to bind Texture with a command buffer with OpenGL selected. This may indicate a bug!");
	}

}
