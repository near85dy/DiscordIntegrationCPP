workspace "discord-1"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    location "build"

project "discord-1"
    kind "StaticLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.h", "src/**.cpp" }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"