#include "pch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>


namespace Hyro {

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t binding, uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_Buffer);
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(TransformData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_Buffer);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void OpenGLUniformBuffer::SetData(void* data)
	{
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Size, data);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
	}

	void OpenGLUniformBuffer::Bind(void* commandBuffer, void* pipelineLayout) const
	{
		HYRO_LOG_CORE_WARN("Tried to bind Unifrom Buffer with command buffer as parameter. This may indicate a bug.");
	}

}
