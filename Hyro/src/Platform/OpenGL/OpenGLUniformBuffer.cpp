#include "pch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>


namespace Hyro {

	OpenGLUniformBuffer::OpenGLUniformBuffer()
	{
		glGenBuffers(1, &m_Buffer);
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_Buffer);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void OpenGLUniformBuffer::SetData(const UniformBufferData& ubo)
	{
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferData), &ubo, GL_DYNAMIC_DRAW);
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
