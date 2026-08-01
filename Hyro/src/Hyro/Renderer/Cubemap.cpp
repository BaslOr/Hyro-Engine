#include "pch.h"
#include "Hyro/Renderer/Cubemap.h"
#include "Hyro/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLCubemap.h"
#include "Platform/Vulkan/VulkanCubemap.h"
#include <numbers>
#include <algorithm>

namespace Hyro {

    Ref<Cubemap> Cubemap::Create(const std::string& filepath)
    {
		switch (SceneRenderer::GetAPI())
		{
		case GraphicsAPIType::None:
			HYRO_LOG_CORE_ERROR("Tried to create a Cubemap without a GraphicsAPI selected!");
			return nullptr;
		case GraphicsAPIType::Vulkan:
			return CreateRef<VulkanCubemap>(filepath);
		case GraphicsAPIType::OpenGL:
			return CreateRef<OpenGLCubemap>(filepath);
		}
    }

	glm::vec3 Cubemap::FaceCoordsToXYZ(int i, int j, int faceID, int faceSize)
	{
		const float A = 2.0f * float(i) / faceSize;
		const float B = 2.0f * float(j) / faceSize;

		if (faceID == 0) return glm::vec3(-1.0f, A - 1.0f, B - 1.0f);
		if (faceID == 1) return glm::vec3(A - 1.0f, -1.0f, 1.0f - B);
		if (faceID == 2) return glm::vec3(1.0f, A - 1.0f, 1.0f - B);
		if (faceID == 3) return glm::vec3(1.0f - A, 1.0f, 1.0f - B);
		if (faceID == 4) return glm::vec3(B - 1.0f, A - 1.0f, 1.0f);
		if (faceID == 5) return glm::vec3(1.0f - B, A - 1.0f, -1.0f);

		return glm::vec3();
	}

	Bitmap Cubemap::ConvertEquirectangularMapToVerticalCross(const Bitmap& bitmap)
	{
		if (bitmap.GetType() != BitmapType::TwoD)
		{
			HYRO_LOG_CORE_ERROR("Failed to convert equirectangular map to vertical corss!");
			return Bitmap();
		}

		uint32_t faceSize = bitmap.GetWidth() / 4;
		uint32_t width = faceSize * 3;
		uint32_t height = faceSize * 4;

		Bitmap result(width, height, 3, bitmap.GetFormat(), BitmapType::TwoD);

		const glm::vec2 kFaceOffsets[] =
		{
			glm::vec2(faceSize, faceSize * 3),
			glm::vec2(0, faceSize),
			glm::vec2(faceSize, faceSize),
			glm::vec2(faceSize * 2, faceSize),
			glm::vec2(faceSize, 0),
			glm::vec2(faceSize, faceSize * 2)
		};

		const int clampW = bitmap.GetWidth() - 1;
		const int clampH = bitmap.GetHeight() - 1;

		for (int face = 0; face != 6; face++)
		{
			for (int i = 0; i != faceSize; i++)
			{
				for (int j = 0; j != faceSize; j++)
				{
					const glm::vec3 P = FaceCoordsToXYZ(i, j, face, faceSize);
					const float R = hypot(P.x, P.y);
					const float theta = atan2(P.y, P.x);
					const float phi = atan2(P.z, R);
					//	float point source coordinates
					const float Uf = float(2.0f * faceSize * (theta + std::numbers::pi) / std::numbers::pi);
					const float Vf = float(2.0f * faceSize * (std::numbers::pi / 2.0f - phi) / std::numbers::pi);
					// 4-samples for bilinear interpolation
					int U1 = std::clamp(static_cast<int>(floor(Uf)), 0, clampW);
					int V1 = std::clamp(static_cast<int>(floor(Vf)), 0, clampH);
					int U2 = std::clamp(U1 + 1, 0, clampW);
					int V2 = std::clamp(V1 + 1, 0, clampH);
					// fractional part
					const float s = Uf - U1;
					const float t = Vf - V1;
					// fetch 4-samples
					glm::vec4 A = bitmap.GetPixel(static_cast<uint32_t>(U1), static_cast<uint32_t>(V1));
					glm::vec4 B = bitmap.GetPixel(static_cast<uint32_t>(U2), static_cast<uint32_t>(V1));
					glm::vec4 C = bitmap.GetPixel(static_cast<uint32_t>(U1), static_cast<uint32_t>(V2));
					glm::vec4 D = bitmap.GetPixel(static_cast<uint32_t>(U2), static_cast<uint32_t>(V2));
					// bilinear interpolation
					const glm::vec4 color = A * (1 - s) * (1 - t) + B * (s) * (1 - t) + C * (1 - s) * t + D * (s) * (t);
					result.SetPixel(i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color);
				}
			};
		}

		return result;
	}

	Bitmap Cubemap::ConvertVerticalCrossToCubeMapFaces(const Bitmap& bitmap)
	{
		uint32_t faceWidth = bitmap.GetWidth() / 3;
		uint32_t faceHeight = bitmap.GetHeight() / 4;

		Bitmap cubemap(faceWidth, faceHeight, 6, bitmap.GetComponents(), bitmap.GetFormat());
		const uint8_t* src = bitmap.GetData();
		uint8_t * dst = cubemap.GetData();
		const uint32_t pixelSize = cubemap.GetComponents() * Bitmap::GetBytesPerComponent(cubemap.GetFormat());


		/*
        	------
        	| +Y |
       ----------------
       | -X | -Z | +X |
       ----------------
        	| -Y |
        	------
        	| +Z |
        	------
		*/


		for (int face = 0; face != 6; ++face) {
			for (int j = 0; j != faceHeight; ++j) {
				for (int i = 0; i != faceWidth; ++i) {
					int x = 0;
					int y = 0;

					switch (face) {
						// CUBE_MAP_POSITIVE_X
					case 0:
						x = 2 * faceWidth + i;
						y = 1 * faceHeight + j;
						break;

						// CUBE_MAP_NEGATIVE_X
					case 1:
						x = i;
						y = faceHeight + j;
						break;

						// CUBE_MAP_POSITIVE_Y
					case 2:
						x = 1 * faceWidth + i;
						y = j;
						break;

						// CUBE_MAP_NEGATIVE_Y
					case 3:
						x = 1 * faceWidth + i;
						y = 2 * faceHeight + j;
						break;

						// CUBE_MAP_POSITIVE_Z
					case 4:
						x = faceWidth + i;
						y = faceHeight + j;
						break;

						// CUBE_MAP_NEGATIVE_Z
					case 5:
						x = 2 * faceWidth - (i + 1);
						y = bitmap.GetHeight() - (j + 1);
						break;
					}

					memcpy(dst, src + (y * bitmap.GetWidth() + x) * pixelSize, pixelSize);

					dst += pixelSize;
				}
			}
		}

		return cubemap;
	}

}
