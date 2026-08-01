#include "pch.h"
#include "Hyro/Utils/Bitmap.h"
#include "Hyro/Core/Core.h"


namespace Hyro
{
	Bitmap::Bitmap(uint32_t width, uint32_t height, uint32_t components, BitmapFormat format, BitmapType type)
		: m_Format(format), m_Type(type), m_Width(width), m_Height(height), m_Components(components),
		m_Data(width* height* components* GetBytesPerComponent(format)), m_Depth(1)
	{
		HYRO_ASSERT(m_Components <= 4);
		HYRO_ASSERT(m_Components > 0);
	}

	Bitmap::Bitmap(uint32_t width, uint32_t height, uint32_t components, BitmapFormat format, BitmapType type, const void* data)
		: m_Format(format), m_Type(type), m_Width(width), m_Height(height), m_Components(components), 
		m_Data(width* height* components* GetBytesPerComponent(format)), m_Depth(1)
	{
		HYRO_ASSERT(m_Components <= 4);
		HYRO_ASSERT(m_Components > 0);
		std::memcpy(m_Data.data(), data, m_Data.size());
	}

	Bitmap::Bitmap(uint32_t width, uint32_t height, uint32_t depth, uint32_t components, BitmapFormat format)
		: m_Type(BitmapType::Cube), m_Format(format), m_Width(width), m_Height(height), m_Components(components),
		m_Data(depth * width* height* components* GetBytesPerComponent(format)), m_Depth(depth)
	{
		HYRO_ASSERT(m_Components <= 4);
		HYRO_ASSERT(m_Components > 0);
		HYRO_ASSERT(m_Depth > 0);
	}

	uint32_t Bitmap::GetBytesPerComponent(BitmapFormat format)
	{
		switch (format)
		{
		case Hyro::BitmapFormat::UnsignedByte:
			return 1;
		case Hyro::BitmapFormat::Float:
			return 4;
		}
	}

	void Bitmap::SetPixel(uint32_t x, uint32_t y, const glm::vec4& color)
	{
		uint64_t offset = (y * m_Width + x) * GetBytesPerComponent(m_Format) * m_Components;

		size_t pixelSize = m_Components * GetBytesPerComponent(m_Format);
		std::memcpy(m_Data.data()+offset, (void*)&color, pixelSize);
	}

	glm::vec4 Bitmap::GetPixel(uint32_t x, uint32_t y) const
	{
		uint64_t offset = (y * m_Width + x) * GetBytesPerComponent(m_Format) * m_Components;
		glm::vec4 color = *(glm::vec4*)((m_Data.data() + offset));

		switch (m_Components)
		{
		case 1:
			return glm::vec4(color.x, 0.f, 0.0f, 0.0f);
		case 2:
			return glm::vec4(color.x, color.y, 0.0f, 0.0f);
		case 3:
			return glm::vec4(color.x, color.y, color.z, 0.0f);
		case 4:
			return glm::vec4(color.x, color.y, color.z, color.a);
		}
	}

	void Bitmap::ChangeComponents(uint32_t newComponentCount)
	{
		HYRO_ASSERT(newComponentCount > 0 && newComponentCount <= 4);

		uint32_t pixelCount = m_Width * m_Height * m_Depth;
		uint32_t bytesPerComponent = GetBytesPerComponent(m_Format);

		std::vector<uint8_t> newData(
			pixelCount * newComponentCount * bytesPerComponent);

		for (uint32_t i = 0; i < pixelCount; i++) {
			uint8_t* src = m_Data.data() +
				i * m_Components * bytesPerComponent;

			uint8_t* dst = newData.data() +
				i * newComponentCount * bytesPerComponent;

			uint32_t copyCount = std::min(m_Components, newComponentCount);

			std::memcpy(
				dst,
				src,
				copyCount * bytesPerComponent
			);

			for (uint32_t c = copyCount; c < newComponentCount; c++) {
				if (m_Format == BitmapFormat::UnsignedByte)	{
					dst[c] = (c == 3) ? 255 : 0;
				}
				else if (m_Format == BitmapFormat::Float) {
					float value = (c == 3) ? 1.0f : 0.0f;

					std::memcpy(
						dst + c * sizeof(float),
						&value,
						sizeof(float)
					);
				}
			}
		}

		m_Components = newComponentCount;
		m_Data = std::move(newData);

	}
}
