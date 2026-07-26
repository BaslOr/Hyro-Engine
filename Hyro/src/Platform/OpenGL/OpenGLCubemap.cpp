#include "pch.h"
#include "Platform/OpenGL/OpenGLCubemap.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

namespace Hyro {

	OpenGLCubemap::OpenGLCubemap(const std::string& filePath)
	{
		int width, height;
		float* data;
		data = stbi_loadf(filePath.c_str(), &width, &height, nullptr, 4);
		if (!data) {
			HYRO_LOG_CORE_ERROR("Failed to load cubemap from file! Path: {0}", filePath.c_str());
			return;
		}
		Bitmap in(width, height, 4, BitmapFormat::Float, BitmapType::TwoD, data);

		Bitmap out = ConvertEquirectangularMapToVerticalCross(in);
		stbi_image_free(data);
		stbi_write_hdr(".cache/screenshot.hdr", out.GetWidth(), out.GetHeight(), out.GetComponents(), (const float*)out.GetData());

		Bitmap cubemap = ConvertVerticalCrossToCubeMapFaces(out);

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		uint32_t faceWidth = cubemap.GetWidth();
		uint32_t faceHeight = cubemap.GetHeight();
		for (uint32_t i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB32F, faceWidth, faceHeight, 0, GL_RGB, GL_FLOAT, cubemap.GetData()+(i * 3 * faceWidth * faceHeight * sizeof(float))
			);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		HYRO_LOG_CORE_TRACE("Created Cubemap");
	}

	OpenGLCubemap::~OpenGLCubemap()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLCubemap::Bind() const
	{
		glBindTextureUnit(0, m_ID);
	}

	void OpenGLCubemap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

}
