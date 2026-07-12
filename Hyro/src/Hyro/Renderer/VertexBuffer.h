#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	class VertexBuffer {
	public:
		virtual void SetData(const std::vector<Vertex>& data) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual void SetLayout(const VertexLayout& layout) = 0;
		virtual VertexLayout GetLayout() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const std::vector<Vertex>& vertices);
	};

}
