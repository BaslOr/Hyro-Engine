#include "pch.h"
#include "Hyro/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include "Hyro/Core/KeyCodes.h"
#include "Hyro/Core/Application.h"

#define HYRO_IMGUI_USE_VULKAN_BACKEND
#include "backends/imgui_impl_glfw.h"
#ifdef HYRO_IMGUI_USE_OPENGL_BACKEND
    #include <backends/imgui_impl_opengl3.h>
#elif defined(HYRO_IMGUI_USE_VULKAN_BACKEND)
    #include <backends/imgui_impl_vulkan.h>
#endif
#include <GLFW/glfw3.h>

namespace Hyro {

	static ImGuiKey HyroKeyCodeToImGuiKeyCode(int keycode)
    {
        switch (keycode)
        {
        case Key::Tab: return ImGuiKey_Tab;
        case Key::Left: return ImGuiKey_LeftArrow;
        case Key::Right: return ImGuiKey_RightArrow;
        case Key::Up: return ImGuiKey_UpArrow;
        case Key::Down: return ImGuiKey_DownArrow;
        case Key::PageUp: return ImGuiKey_PageUp;
        case Key::PageDown: return ImGuiKey_PageDown;
        case Key::Home: return ImGuiKey_Home;
        case Key::End: return ImGuiKey_End;
        case Key::Insert: return ImGuiKey_Insert;
        case Key::Delete: return ImGuiKey_Delete;
        case Key::Backspace: return ImGuiKey_Backspace;
        case Key::Space: return ImGuiKey_Space;
        case Key::Enter: return ImGuiKey_Enter;
        case Key::Escape: return ImGuiKey_Escape;
        case Key::Apostrophe: return ImGuiKey_Apostrophe;
        case Key::Comma: return ImGuiKey_Comma;
        case Key::Minus: return ImGuiKey_Minus;
        case Key::Period: return ImGuiKey_Period;
        case Key::Slash: return ImGuiKey_Slash;
        case Key::Semicolon: return ImGuiKey_Semicolon;
        case Key::Equal: return ImGuiKey_Equal;
        case Key::LeftBracket: return ImGuiKey_LeftBracket;
        case Key::Backslash: return ImGuiKey_Backslash;
        case Key::World1: return ImGuiKey_Oem102;
        case Key::World2: return ImGuiKey_Oem102;
        case Key::RightBracket: return ImGuiKey_RightBracket;
        case Key::GraveAccent: return ImGuiKey_GraveAccent;
        case Key::CapsLock: return ImGuiKey_CapsLock;
        case Key::ScrollLock: return ImGuiKey_ScrollLock;
        case Key::NumLock: return ImGuiKey_NumLock;
        case Key::PrintScreen: return ImGuiKey_PrintScreen;
        case Key::Pause: return ImGuiKey_Pause;
        case Key::KP0: return ImGuiKey_Keypad0;
        case Key::KP1: return ImGuiKey_Keypad1;
        case Key::KP2: return ImGuiKey_Keypad2;
        case Key::KP3: return ImGuiKey_Keypad3;
        case Key::KP4: return ImGuiKey_Keypad4;
        case Key::KP5: return ImGuiKey_Keypad5;
        case Key::KP6: return ImGuiKey_Keypad6;
        case Key::KP7: return ImGuiKey_Keypad7;
        case Key::KP8: return ImGuiKey_Keypad8;
        case Key::KP9: return ImGuiKey_Keypad9;
        case Key::KPDecimal: return ImGuiKey_KeypadDecimal;
        case Key::KPDivide: return ImGuiKey_KeypadDivide;
        case Key::KPMultiply: return ImGuiKey_KeypadMultiply;
        case Key::KPSubtract: return ImGuiKey_KeypadSubtract;
        case Key::KPAdd: return ImGuiKey_KeypadAdd;
        case Key::KPEnter: return ImGuiKey_KeypadEnter;
        case Key::KPEqual: return ImGuiKey_KeypadEqual;
        case Key::LeftShift: return ImGuiKey_LeftShift;
        case Key::LeftControl: return ImGuiKey_LeftCtrl;
        case Key::LeftAlt: return ImGuiKey_LeftAlt;
        case Key::LeftSuper: return ImGuiKey_LeftSuper;
        case Key::RightShift: return ImGuiKey_RightShift;
        case Key::RightControl: return ImGuiKey_RightCtrl;
        case Key::RightAlt: return ImGuiKey_RightAlt;
        case Key::RightSuper: return ImGuiKey_RightSuper;
        case Key::Menu: return ImGuiKey_Menu;
        case Key::D0: return ImGuiKey_0;
        case Key::D1: return ImGuiKey_1;
        case Key::D2: return ImGuiKey_2;
        case Key::D3: return ImGuiKey_3;
        case Key::D4: return ImGuiKey_4;
        case Key::D5: return ImGuiKey_5;
        case Key::D6: return ImGuiKey_6;
        case Key::D7: return ImGuiKey_7;
        case Key::D8: return ImGuiKey_8;
        case Key::D9: return ImGuiKey_9;
        case Key::A: return ImGuiKey_A;
        case Key::B: return ImGuiKey_B;
        case Key::C: return ImGuiKey_C;
        case Key::D: return ImGuiKey_D;
        case Key::E: return ImGuiKey_E;
        case Key::F: return ImGuiKey_F;
        case Key::G: return ImGuiKey_G;
        case Key::H: return ImGuiKey_H;
        case Key::I: return ImGuiKey_I;
        case Key::J: return ImGuiKey_J;
        case Key::K: return ImGuiKey_K;
        case Key::L: return ImGuiKey_L;
        case Key::M: return ImGuiKey_M;
        case Key::N: return ImGuiKey_N;
        case Key::O: return ImGuiKey_O;
        case Key::P: return ImGuiKey_P;
        case Key::Q: return ImGuiKey_Q;
        case Key::R: return ImGuiKey_R;
        case Key::S: return ImGuiKey_S;
        case Key::T: return ImGuiKey_T;
        case Key::U: return ImGuiKey_U;
        case Key::V: return ImGuiKey_V;
        case Key::W: return ImGuiKey_W;
        case Key::X: return ImGuiKey_X;
        case Key::Y: return ImGuiKey_Y;
        case Key::Z: return ImGuiKey_Z;
        case Key::F1: return ImGuiKey_F1;
        case Key::F2: return ImGuiKey_F2;
        case Key::F3: return ImGuiKey_F3;
        case Key::F4: return ImGuiKey_F4;
        case Key::F5: return ImGuiKey_F5;
        case Key::F6: return ImGuiKey_F6;
        case Key::F7: return ImGuiKey_F7;
        case Key::F8: return ImGuiKey_F8;
        case Key::F9: return ImGuiKey_F9;
        case Key::F10: return ImGuiKey_F10;
        case Key::F11: return ImGuiKey_F11;
        case Key::F12: return ImGuiKey_F12;
        case Key::F13: return ImGuiKey_F13;
        case Key::F14: return ImGuiKey_F14;
        case Key::F15: return ImGuiKey_F15;
        case Key::F16: return ImGuiKey_F16;
        case Key::F17: return ImGuiKey_F17;
        case Key::F18: return ImGuiKey_F18;
        case Key::F19: return ImGuiKey_F19;
        case Key::F20: return ImGuiKey_F20;
        case Key::F21: return ImGuiKey_F21;
        case Key::F22: return ImGuiKey_F22;
        case Key::F23: return ImGuiKey_F23;
        case Key::F24: return ImGuiKey_F24;
        default: return ImGuiKey_None;
        }
    }


	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{	}

	void ImGuiLayer::OnAttach()
	{
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        //style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        //style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
        io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
        io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetNative());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef HYRO_IMGUI_USE_OPENGL_BACKEND
        ImGui_ImplOpenGL3_Init("#version 460");
#elif defined(HYRO_IMGUI_USE_VULKAN_BACKEND)
        //ImGui_ImplGlfw_InitForVulkan(window, true);
#endif
	}

	void ImGuiLayer::OnDetach()
	{
#ifdef HYRO_IMGUI_USE_OPENGL_BACKEND
        ImGui_ImplOpenGL3_Shutdown();
#elif defined(HYRO_IMGUI_USE_VULKAN_BACKEND)
        //ImGui_ImplVulkan_Shutdown();
#endif
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImGuiLayer::Begin()
    {
        // Start the Dear ImGui frame
#ifdef HYRO_IMGUI_USE_OPENGL_BACKEND
        ImGui_ImplOpenGL3_NewFrame();
#elif defined(HYRO_IMGUI_USE_VULKAN_BACKEND)
        //ImGui_ImplVulkan_NewFrame();
#endif
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = { static_cast<float>(app.GetWindow()->GetWidth()), static_cast<float>(app.GetWindow()->GetWidth()) };

        ImGui::Render();
#ifdef HYRO_IMGUI_USE_OPENGL_BACKEND
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(HYRO_IMGUI_USE_VULKAN_BACKEND)
        //VkCommandBuffer commandBuffer;
        //ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
#endif

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}
