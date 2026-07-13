#include "pch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Hyro {

	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexLayout& layout, uint32_t vertexCountHint)
		:m_Layout(layout)
	{
		glCreateBuffers(1, &m_ID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, layout.GetStride() * vertexCountHint, nullptr, GL_DYNAMIC_DRAW);
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

}
