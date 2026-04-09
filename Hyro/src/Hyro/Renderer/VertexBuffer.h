#pragma once
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Core/Memory.h"

namespace Hyro {

	class VertexBuffer {
	public:
		virtual void SetData(const std::vector<Vertex>& data) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<VertexBuffer> Create();
		static Ref<VertexBuffer> Create(std::vector<Vertex> vertices);

	private:
	};

}
