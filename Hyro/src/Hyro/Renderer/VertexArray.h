#pragma once
#include "Hyro/Core/Memory.h"	
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace Hyro {


	class VertexArray {
	public:
		enum TYPE {
			FLOAT
		};

	public:
		static Ref<VertexArray> Create();

		virtual void AddVertexBuffer(Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> buffer) = 0;

		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;//Only for Vulkan

	};



}
