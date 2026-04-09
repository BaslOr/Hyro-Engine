#pragma once 
#include "Hyro/Renderer/IndexBuffer.h"

#include <iostream>

namespace Hyro {

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer();
		OpenGLIndexBuffer(const std::vector<uint32_t>& data);
		~OpenGLIndexBuffer();

		virtual void SetData(const std::vector<uint32_t>& data) override;

		virtual void Bind() const override;
		virtual void UnBind() const override;

	private:
		uint32_t m_ID;
	};

}
