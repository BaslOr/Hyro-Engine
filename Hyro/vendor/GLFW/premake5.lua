project "GLFW"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "include/GLFW/**.h",
        "src/**.c",
        "src/**.h"
    }

    includedirs {
        "Hyro/vendor/GLFW/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32" }
        links { "opengl32" }

    filter "system:linux"
        defines { "_GLFW_X11" }
        links { "X11", "pthread", "dl", "GL" }

    filter "system:macosx"
        defines { "_GLFW_COCOA" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "OpenGL.framework" }

    filter {}