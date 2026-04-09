workspace "Hyro"
   configurations { "Debug", "Release", "Dist" }
   architecture "x64"
   startproject "Sandbox"

function SetupCppProject()
    language "C++"
    cppdialect "C++23"
    staticruntime "Off"
    warnings "Extra"
end

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")


filter "system:windows"
   systemversion "latest"
   defines { "PLATFORM_WINDOWS"}

filter "system:linux"
   pic "On"
   defines { "PLATFORM_LINUX" }

filter "system:macosx"
   defines { "PLATFORM_MAC" }
   
filter "configurations:Debug"
   defines { "HYRO_DEBUG" }
   runtime "Debug"
   symbols "On"

filter "configurations:Release"
   defines { "HYRO_RELEASE" }
   runtime "Release"
   optimize "On"

filter "configurations:Dist"
   defines { "HYRO_DIST" }
   runtime "Release"
   optimize "Full"
   symbols "Off"

filter {}

group "Dependencies"
include "Hyro/vendor/GLFW"
include "Hyro/vendor/glad"
include "Hyro/vendor/ImGui"
include "Hyro/vendor/spdlog"
group ""


project "Hyro"
   location "Hyro"
   kind "StaticLib"
   SetupCppProject()

  	pchheader "pch.h"
  	pchsource "Hyro/src/Hyro/pch.cpp"


   files {
      "Hyro/src/**.h",
      "Hyro/src/**.cpp",
      "Hyro/src/**.c" 
   }


   links { 
       "GLFW",
       "glad",
       "ImGui",
       "spdlog"
   }
   includedirs { 
       "Hyro/vendor/glm",
       "Hyro/vendor/KHR",
       "Hyro/vendor/glad",
       "Hyro/vendor/glad/include",
       "Hyro/vendor/GLFW/include",
       "Hyro/vendor/ImGui",
       "Hyro/vendor/spdlog/include",
       "Hyro/src"
   }

   defines { "SPDLOG_COMPILED_LIB" }

   filter { "system:windows", "action:vs*" }  -- This is needed that spdlog compiles successfully
      buildoptions { "/utf-8" }
   filter{}

   filter "system:windows"
      links "opengl32"
   filter {}


project "Sandbox"
   location "Sandbox"
   kind "ConsoleApp"
   SetupCppProject()

   files {
      "Sandbox/src/**.h",
      "Sandbox/src/**.cpp",
      "Sandbox/src/**.c" 
   }

   defines { "SPDLOG_COMPILED_LIB" }

   links { 
      "Hyro"
   }

   includedirs { 
       "Hyro/vendor/glm",
       "Hyro/vendor/KHR",
       "Hyro/vendor/glad",
       "Hyro/vendor/glad/include",
       "Hyro/vendor/GLFW/include",
       "Hyro/vendor/ImGui",
       "Hyro/vendor/spdlog/include",
       "Hyro/src"
   }

   filter { "system:windows", "action:vs*" }  -- This is needed that spdlog compiles successfully
      buildoptions { "/utf-8" }
   filter {}
    