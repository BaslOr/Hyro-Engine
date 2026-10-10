#pragma once 
#include "Hyro/Renderer/RenderingObjects/IndexBuffer.h"


namespace Hyro {

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer();
		OpenGLIndexBuffer(const std::vector<uint32_t>& data);
		~OpenGLIndexBuffer();

		void SetData(const std::vector<uint32_t>& data) override;

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		uint32_t GetCount() const override;

	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};

}
