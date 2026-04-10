#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hyro {

	OpenGLVertexArray::OpenGLVertexArray()
		: m_ID(0)
	{
		glCreateVertexArrays(1, &m_ID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetVertexAttribPointer(uint32_t index, int size, TYPE type, int stride, int pointer)
	{
		Bind();
		glVertexAttribPointer(index, size, InternalTypeToGLType(type), GL_FALSE, stride, (void*)pointer);
	}

	void OpenGLVertexArray::EnableVertexAttribArray(uint16_t index)
	{
		Bind();
		glEnableVertexAttribArray(index);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> buffer)
	{
		Bind();
		buffer->Bind();
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> buffer)
	{
		Bind();
		buffer->Bind();
	}

	int OpenGLVertexArray::InternalTypeToGLType(TYPE type)
	{
		switch (type)
		{
		case VertexArray::FLOAT:
			return GL_FLOAT;
			break;
		}
	}
}