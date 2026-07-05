#include "pch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Hyro {


	OpenGLIndexBuffer::OpenGLIndexBuffer()
	{
		glCreateBuffers(1, &m_ID);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& data)
		: m_ID(0)
	{
		glCreateBuffers(1, &m_ID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);
	}

	void OpenGLIndexBuffer::SetData(const std::vector<uint32_t>& data)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Bind(void* commandBuffer) const
	{
		HYRO_LOG_CORE_WARN("IndexBuffer::Bind(...) was called with a commandBuffer on OpenGL side. This may indicate a Bug!");
	}

}
