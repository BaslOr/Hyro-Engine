#include "pch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Hyro {

	OpenGLVertexBuffer::OpenGLVertexBuffer()
	{
		glCreateBuffers(1, &m_ID);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& data)
		: m_ID(0)
	{
		glCreateBuffers(1, &m_ID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex>& data)
		: m_ID(0)
	{
		glCreateBuffers(1, &m_ID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::SetData(const std::vector<Vertex>& data)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
