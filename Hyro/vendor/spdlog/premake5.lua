project "spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "include/**.h",
        "src/**.cpp"
    }

    includedirs {
        "include"
    }

    defines {
        "SPDLOG_COMPILED_LIB"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter "system:linux"
        links { "pthread" }

    filter "system:macosx"
        -- no extra links needed

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter { "system:windows", "action:vs*" }  -- This is needed that spdlog compiles successfully
        buildoptions { "/utf-8" }
    filter {}