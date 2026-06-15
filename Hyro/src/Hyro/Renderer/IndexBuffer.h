#pragma once
#include "Hyro/Core/Memory.h"

#include <vector>

namespace Hyro {

	class IndexBuffer {
	public:
		static Ref<IndexBuffer> Create();
		static Ref<IndexBuffer> Create(const std::vector<uint32_t>& indices);

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual void SetData(const std::vector<uint32_t>& data) = 0;
	};

}
