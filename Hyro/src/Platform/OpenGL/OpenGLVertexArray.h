#pragma once
#include "Hyro/Renderer/VertexArray.h"

#include <cstdint>

namespace Hyro {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void UnBind() const override;

		void SetVertexAttribPointer(uint32_t index, int size, TYPE type, int stride, int pointer) override;
		void EnableVertexAttribArray(uint16_t index) override;

		void AddVertexBuffer(Ref<VertexBuffer> buffer) override;
		void SetIndexBuffer(Ref<IndexBuffer> buffer) override;

	private:
		uint32_t m_ID;

		int InternalTypeToGLType(TYPE type);
	};
	
}