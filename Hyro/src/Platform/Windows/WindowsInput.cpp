#include "pch.h"
#include "Platform/Windows/WindowsInput.h"
#include "Hyro/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hyro {

	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow()->GetNative();

		auto state = glfwGetKey(window, keyCode);
		return (state == GLFW_PRESS) || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow()->GetNative();

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow()->GetNative();

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow()->GetNative();

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)y;
	}

}
