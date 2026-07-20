#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Core/Memory.h"
#include <cstdint>

namespace Hyro {

	class VertexBuffer {
	public:
		virtual void SetData(const std::vector<Vertex2D>& data) = 0;
		virtual void SetData(const std::vector<Vertex3D>& data) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual VertexLayout GetLayout() const = 0;

		static Ref<VertexBuffer> Create(const VertexLayout& layout, uint32_t vertexCountHint);
	};

}
