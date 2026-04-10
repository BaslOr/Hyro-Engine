#pragma once

#include "Hyro/Core/Memory.h"	
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Hyro {

	class VertexArray {
	public:
		enum TYPE {
			FLOAT
		};

	public:
		static Ref<VertexArray> Create();

		virtual void SetVertexAttribPointer(uint32_t index, int size, TYPE type, int stride, int pointer) = 0;
		virtual void EnableVertexAttribArray(uint16_t incex) = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> buffer) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

	};

}
