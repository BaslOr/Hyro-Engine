project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "glad/glad.c",
        "glad/glad.h",
        "include/**.h"
    }

    includedirs {
        "include/",
        "../glad"
    }

    filter "system:linux"
        pic "On"

    filter {}
