#include "pch.h"
#include "Platform/OpenGL/OpenGLAPI.h"

#include "Hyro/Core/Core.h"
#include "Hyro/Project/AssetManager.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>


namespace Hyro {

    static void GLAPIENTRY OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam)
    {
        std::stringstream ss;
        ss << "[OpenGL Debug] ";

        // Source
        switch (source) {
        case GL_DEBUG_SOURCE_API:             ss << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     ss << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           ss << "Other"; break;
        }

        ss << " | ";

        // Type
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:               ss << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         ss << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         ss << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              ss << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           ss << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               ss << "Other"; break;
        }

        ss << " | Severity: ";

        // Severity
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         ss << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       ss << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          ss << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "Notification"; break;
        }

        ss << " | ID: " << id << " | Message: " << message;

        const std::string output = ss.str();

        // Use Core logger based on severity
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            HYRO_LOG_CORE_ERROR("{}", output);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            HYRO_LOG_CORE_WARN("{}", output);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            HYRO_LOG_CORE_INFO("{}", output);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            HYRO_LOG_CORE_TRACE("{}", output);
            break;
        default:
            HYRO_LOG_CORE_INFO("{}", output);
            break;
        }
    }

    OpenGLAPI::OpenGLAPI()
		: m_FramebufferData(OpenGLFramebuffer(1280, 720))
    {
        if (g_CurrentBuildConfig == BuildConfig::Debug)
            SetupDebugCallback();


        std::vector<FramebufferVertex> quadVertices = {
            { { 1.0f,  1.0f }, {  1.0f,  1.0f, } },
            { { 1.0f, -1.0f }, {  1.0f,  0.0f, } },
            { {-1.0f, -1.0f }, {  0.0f,  0.0f, } },
            { {-1.0f,  1.0f }, {  0.0f,  1.0f  } }
        };
        std::vector<uint32_t> quadIndices = {
            0, 1, 2,
            2, 3, 0
        };
		m_FramebufferData.ColorAttachment = CreateRef<OpenGLTexture>(m_FramebufferData.Framebuffer.GetColorAttachmentRendererID());
        m_FramebufferData.Shader = AssetManager::GetShader("OpenGLFramebuffer");
        m_FramebufferData.VertexArray = VertexArray::Create();
        m_FramebufferData.VertexBuffer = VertexBuffer::Create(m_FramebufferData.Shader->GetVertexLayout(), static_cast<uint32_t>(quadVertices.size()));
        m_FramebufferData.VertexBuffer->SetData(quadVertices.data(), quadVertices.size() * sizeof(FramebufferVertex));
        m_FramebufferData.IndexBuffer = IndexBuffer::Create(static_cast<uint32_t>(quadIndices.size()) * sizeof(uint32_t));
        m_FramebufferData.IndexBuffer->SetData(quadIndices);

        m_FramebufferData.VertexArray->AddVertexBuffer(m_FramebufferData.VertexBuffer);
        m_FramebufferData.VertexArray->SetIndexBuffer(m_FramebufferData.IndexBuffer);

        m_FramebufferData.Material = Material::Create(m_FramebufferData.Shader);
        m_FramebufferData.Material->SetSampler(AssetManager::GetFallbackTexture(), 0);
    }

    void OpenGLAPI::BeginRenderPass()
    {
        m_FramebufferData.Framebuffer.Bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
    }

    void OpenGLAPI::EndRenderPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glDisable(GL_DEPTH_TEST);
		m_FramebufferData.Material->SetSampler(m_FramebufferData.ColorAttachment, 0);
		Submit(m_FramebufferData.VertexArray, m_FramebufferData.Material, 6);
    }

    void OpenGLAPI::Submit(Ref<VertexArray> vao, Ref<Material> material, uint32_t count)
	{
        vao->Bind();
        material->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

    void OpenGLAPI::SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap)
    {
        vertexArray->Bind();
        material->Bind();
        cubemap->Bind();

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        cubemap->Unbind();
    }

	void OpenGLAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLAPI::SetupDebugCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // damit der Callback direkt aufgerufen wird
		glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

}