#include "pch.h"
#include "WindowsWindow.h"

#include "Hyro/Events/KeyEvent.h"
#include "Hyro/Events/MouseEvent.h"
#include "Hyro/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Core/Core.h"

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

    Ref<Window> Window::Create(const WindowProps& props) {
        return CreateRef<WindowsWindow>(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        if (!glfwInit()) {
			HYRO_LOG_CORE_ERROR("Failed to initialize GLFW!");
        }

        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.Title = props.Height;
        
        if (Renderer::GetAPI() == GraphicsAPIType::OpenGL) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
			HYRO_LOG_CORE_ERROR("Failed to create Window!");
        }

		//TODO: Create Graphics Context based on Renderer::GetAPI()
        m_Data.GraphicsContext = CreateRef<OpenGLContext>(m_Window);
        m_Data.GraphicsContext->Init();

        //Move to GraphicsContext
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // damit der Callback direkt aufgerufen wird
        glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        m_Data.GraphicsContext->ResizeViewport(m_Data.Width, m_Data.Height);


        glfwSetWindowUserPointer(m_Window, &m_Data);


        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
                data.EventCallback(event);

                if(data.GraphicsContext != nullptr)
                    data.GraphicsContext->ResizeViewport(data.Width, data.Height);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void WindowsWindow::OnUpdate(TimeStep deltaTime)
    {
        glfwPollEvents();
        m_Data.GraphicsContext->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        
        m_Data.VSync = enabled;
    }
}
