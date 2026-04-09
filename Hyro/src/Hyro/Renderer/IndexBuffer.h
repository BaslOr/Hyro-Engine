#pragma once
#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Renderer.h"

#include <vector>

namespace Hyro {

	class IndexBuffer {
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(const std::vector<uint32_t>& data) = 0;

		static Ref<IndexBuffer> Create();
		static Ref<IndexBuffer> Create(const std::vector<uint32_t>& indices);
	};

}
