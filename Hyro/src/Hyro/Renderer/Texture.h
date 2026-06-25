#pragma once
#include "Hyro/Core/Memory.h"

#include <string>

namespace Hyro {

	class Texture {
	public:
		static Ref<Texture> Load(const std::string& filePath);
	
		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual uint32_t GetSpriteIndex() const = 0;

	};

}

