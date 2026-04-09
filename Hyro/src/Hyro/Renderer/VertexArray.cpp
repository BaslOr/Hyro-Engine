#include "pch.h"
#include "VertexArray.h"

namespace Hyro {

	VertexArray::VertexArray()
		: m_ID(0)
	{
		glCreateVertexArrays(1, &m_ID);
		bind();
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::setVertexAttribPointer(uint32_t index, int size, TYPE type, int stride, int pointer) const
	{
		bind();
		glVertexAttribPointer(index, size, internalTypeToGLType(type), GL_FALSE, stride, (void*)pointer);
	}

	void VertexArray::enableVertexAttribArray(uint16_t index) const
	{
		bind();
		glEnableVertexAttribArray(index);
	}

	void VertexArray::addVertexBuffer(Ref<VertexBuffer> buffer)
	{
		bind();
		buffer->Bind();
	}

	void VertexArray::setIndexBuffer(Ref<IndexBuffer> buffer)
	{
		bind();
		buffer->Bind();
	}

	int VertexArray::internalTypeToGLType(TYPE type) const
	{
		switch (type)
		{
		case VertexArray::FLOAT:
			return GL_FLOAT;
			break;
		}
	}

}
