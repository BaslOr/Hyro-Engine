#pragma once
#include <glad/glad.h>

#include <iostream>

#include "Hyro/Core/Memory.h"	
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Hyro {

	class VertexArray {
	public:
		enum TYPE {
			FLOAT
		};

		VertexArray();
		~VertexArray();

		void setVertexAttribPointer(uint32_t index, int size, TYPE type, int stride, int pointer) const;
		void enableVertexAttribArray(uint16_t position) const;

		void addVertexBuffer(Ref<VertexBuffer> buffer);
		void setIndexBuffer(Ref<IndexBuffer> buffer);

		void bind() const { glBindVertexArray(m_ID); }
		void unBind() const { glBindVertexArray(0); }

	private:
		uint32_t m_ID;

		int internalTypeToGLType(TYPE type) const;

	};

}
