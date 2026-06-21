#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hyro {

	OpenGLVertexArray::OpenGLVertexArray()
		: m_ID(0)
	{
		glGenVertexArrays(1, &m_ID);
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

	void OpenGLVertexArray::SetLayout(const VertexLayout& layout)
	{
		uint32_t i = 0;
		uint32_t offset = 0;
		for (const auto type : layout.GetVertexAttributes()) {
			glVertexAttribPointer(i, AttributeTypeToAttributeSize(type), AttributeTypeToOpenGLEnum(type), GL_FALSE, layout.GetStride(), (void*)offset);
			glEnableVertexAttribArray(i);

			offset += layout.GetVertexAttributeSize(type);
			i++;
		}
	}

	int OpenGLVertexArray::AttributeTypeToOpenGLEnum(VertexAttributeType type) const
	{
		switch (type)
		{
		case Hyro::VertexAttributeType::FLOAT:
			return GL_FLOAT;
			break;
		case Hyro::VertexAttributeType::FLOAT2:
			return GL_FLOAT;
			break;
		case Hyro::VertexAttributeType::FLOAT3:
			return GL_FLOAT;
			break;
		case Hyro::VertexAttributeType::FLOAT4:
			return GL_FLOAT;
			break;
		}
	}

	uint32_t OpenGLVertexArray::AttributeTypeToAttributeSize(VertexAttributeType type) const
	{
		switch (type)
		{
		case Hyro::VertexAttributeType::FLOAT:
			return 1;
			break;
		case Hyro::VertexAttributeType::FLOAT2:
			return 2;
			break;
		case Hyro::VertexAttributeType::FLOAT3:
			return 3;
			break;
		case Hyro::VertexAttributeType::FLOAT4:
			return 4;
			break;
		}
	}
}