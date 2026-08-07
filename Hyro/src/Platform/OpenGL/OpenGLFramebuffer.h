#pragma once
#include <cstdint>

namespace Hyro {

	class OpenGLFramebuffer {
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		~OpenGLFramebuffer();

		void Bind();
		inline uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }


	private:
		uint32_t m_Framebuffer;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
	};

}