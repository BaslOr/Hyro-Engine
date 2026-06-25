project "stb_image"
    kind "StaticLib"
    language "C"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "stb_image.h",
        "stb_image.cpp"
    }

    filter "system:linux"
        pic "On"

    filter {}
