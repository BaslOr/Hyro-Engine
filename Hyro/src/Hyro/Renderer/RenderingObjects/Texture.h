#pragma once
#include "Hyro/Core/Memory.h"

#include <string>

namespace Hyro {

	class Texture {
	public:
		static Ref<Texture> Load(const std::string& filePath);
	
		virtual void Bind(uint32_t slot) const = 0;
		virtual void Bind(void* commandBuffer) const = 0;
	};

}

