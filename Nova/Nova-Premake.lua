NEOINFUSED = "../NeoInfused"

project "Nova"
    location "./"
    targetname "%{prj.name}-bin"
    kind "ConsoleApp"
    staticruntime "off"

    language "C++"
    cppdialect "C++20"
    systemversion "latest"

    pchheader "nova_pch.hpp"
    pchsource "src/nova_pch.cpp"

    files {
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs {
        "src/",
        "%{NEOINFUSED}/include/",
        "%{NEOINFUSED}/%{INCLUDE_DIR.GLFW}",
        "%{NEOINFUSED}/%{INCLUDE_DIR.glad}",
        "%{NEOINFUSED}/%{INCLUDE_DIR.glm}"
    }
    links {
        "NeoInfused",
        "GLFW",
        "glad",
        "Xcursor",
        "Xinerama",
        "Xrandr",
        "Xi",
        "dl",
        "X11",
        "m"
    }

    filter "system:linux"
        toolset "gcc"
        defines { "NEO_PLATFORM_LINUX" }
    filter "system:windows"
        toolset "msc"
        defines {
            "NEO_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter { "configurations:dist", "system:windows" }
        kind "WindowedApp"

    filter "configurations:dbg"
        symbols "On"
        runtime "Debug"
        defines { "NEO_CONFIG_DEBUG" }
    filter "configurations:rel"
        optimize "speed"
        symbols "off"
        runtime "Release"
        defines { "NEO_CONFIG_RELEASE" }
    filter "configurations:dist"
        optimize "speed"
        symbols "off"
        runtime "Release"
        defines { "NEO_CONFIG_DIST" }