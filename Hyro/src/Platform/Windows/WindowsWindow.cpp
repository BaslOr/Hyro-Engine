#include "pch.h"
#include "WindowsWindow.h"

#include "Hyro/Events/KeyEvent.h"
#include "Hyro/Events/MouseEvent.h"
#include "Hyro/Events/ApplicationEvent.h"

#include "Hyro/Renderer/Renderer.h"

#include "Hyro/Core/Core.h"

namespace Hyro {

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        if (!glfwInit()) {
			HYRO_LOG_CORE_ERROR("Failed to initialize GLFW!");
        }

        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.Title = props.Title;
        
        SetupWindowHints();

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
			HYRO_LOG_CORE_ERROR("Failed to create Window!");
        }

        m_Data.GraphicsContext = GraphicsContext::Create(Renderer::GetAPI(), GetNative());
        m_Data.GraphicsContext->Init();
        m_Data.GraphicsContext->ResizeViewport(m_Data.Width, m_Data.Height);
        

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetupGLFWCallbacks();
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

    void WindowsWindow::SetupWindowHints()
    {
        switch (Renderer::GetAPI())
        {
        case GraphicsAPIType::None:
            HYRO_LOG_CORE_FATAL("No Graphics API selected!");
            break;
        case GraphicsAPIType::OpenGL:
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);
            break;
        case GraphicsAPIType::Vulkan:
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            break;
        }
    }

    void WindowsWindow::SetupGLFWCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = static_cast<uint16_t>(width);
                data.Height = static_cast<uint16_t>(height);

                WindowResizeEvent event(width, height);
                data.EventCallback(event);

                if (data.GraphicsContext != nullptr)
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
                    KeyPressedEvent event((KeyCode)key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((KeyCode)key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event((KeyCode)key, true);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event((KeyCode)keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event((MouseCode)button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event((MouseCode)button);
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
}
