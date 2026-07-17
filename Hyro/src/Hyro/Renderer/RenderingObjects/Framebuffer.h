#pragma once
#include "Hyro/Core/Memory.h"

namespace Hyro {
	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(uint32_t width, uint32_t height);
	};

}
