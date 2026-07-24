#pragma once
#include <vector>
#include <cstdint>

#include <glm/fwd.hpp>

namespace Hyro {

	enum class BitmapFormat {
		UnsignedByte, Float
	};

	enum class BitmapType {
		TwoD, Cube
	};

	class Bitmap {
	public:
		Bitmap() = default;
		Bitmap(uint32_t width, uint32_t height, uint32_t components, BitmapFormat format, BitmapType type);
		Bitmap(uint32_t width, uint32_t height, uint32_t components, BitmapFormat format, BitmapType type, const void* data);
		Bitmap(uint32_t width, uint32_t height, uint32_t depth, uint32_t components, BitmapFormat format);

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline uint32_t GetComponents() const { return m_Components; }

		inline BitmapType GetType() const { return m_Type; }
		inline BitmapFormat GetFormat() const { return m_Format; }

		inline uint8_t* GetData() { return m_Data.data(); }
		inline const uint8_t* GetData() const { return m_Data.data(); }

		static uint32_t GetBytesPerComponent(BitmapFormat format);
		void SetPixel(uint32_t x, uint32_t y, const glm::vec4& color);
		glm::vec4 GetPixel(uint32_t x, uint32_t y) const;

	private:
		std::vector<uint8_t> m_Data;
		uint32_t m_Width, m_Height, m_Components, m_Depth;

		BitmapType m_Type;
		BitmapFormat m_Format;
	};

}