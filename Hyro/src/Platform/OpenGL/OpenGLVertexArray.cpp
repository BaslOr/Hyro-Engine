#include "pch.h"
#include <glad/glad.h>
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Hyro/Renderer/Utils/ShaderUtils.h"


namespace Hyro {

	OpenGLVertexArray::OpenGLVertexArray()
		: m_ID(0)
	{
		glGenVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
		m_IndexBuffer->Bind();
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		const auto& layout = buffer->GetLayout();

		for (const auto& element : layout.GetElements())
		{
			glEnableVertexAttribArray(element.Location);

			glVertexAttribPointer(
				element.Location,
				ShaderUtils::GetCountFromShaderType(element.Type),
				AttributeTypeToOpenGLEnum(element.Type),
				GL_FALSE,
				layout.GetStride(),
				(void*)element.Offset
			);
		}
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> buffer)
	{
		m_IndexBuffer = buffer;
		Bind();
		buffer->Bind();
	}

	uint32_t OpenGLVertexArray::AttributeTypeToOpenGLEnum(VertexAttributeType type) const
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
}