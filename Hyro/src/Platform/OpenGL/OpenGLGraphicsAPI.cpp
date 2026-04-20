#include "pch.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

#include "Hyro/Core/Core.h"

#include <glad/glad.h>

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

    OpenGLGraphicsAPI::OpenGLGraphicsAPI()
    {
        if (g_CurrentBuildConfig == BuildConfig::Debug)
            SetupDebugCallback();
    }

    void OpenGLGraphicsAPI::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);
	}

	void OpenGLGraphicsAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::SetupDebugCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // damit der Callback direkt aufgerufen wird
		glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

}