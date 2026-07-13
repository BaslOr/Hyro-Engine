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
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> buffer)
	{
		Bind();
		buffer->Bind();
		uint32_t i = 0;
		auto layout = buffer->GetLayout();
		for (const auto& element : layout.GetElements()) {
			glVertexAttribPointer(i, ShaderUtils::GetCountFromShaderType(element.Type), AttributeTypeToOpenGLEnum(element.Type),
				GL_FALSE, layout.GetStride(), (void*)element.Offset);
			glEnableVertexAttribArray(i);

			i++;
		}
		
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> buffer)
	{
		Bind();
		buffer->Bind();
	}

	int OpenGLVertexArray::AttributeTypeToOpenGLEnum(ShaderType type) const
	{
		switch (type)
		{
		case Hyro::ShaderType::FLOAT:
			return GL_FLOAT;
			break;
		case Hyro::ShaderType::FLOAT2:
			return GL_FLOAT;
			break;
		case Hyro::ShaderType::FLOAT3:
			return GL_FLOAT;
			break;
		case Hyro::ShaderType::FLOAT4:
			return GL_FLOAT;
			break;
		}
	}
}