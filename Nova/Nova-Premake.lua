NEOINFUSED = "../NeoInfused"
INFLUX_VK_BIN = "../bin/%{OUTPUT_DIR}/Influx-vk/Influx-vk-bin"
--INFLUX_GL_BIN = "../bin/%{OUTPUT_DIR}/Influx-gl/Influx-gl-bin"

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
        "%{NEOINFUSED}/%{INCLUDE_DIR.glm}",
        "%{NEOINFUSED}/%{INCLUDE_DIR.inf}",
        "%{NEOINFUSED}/%{INCLUDE_DIR.stb}"
    }
    links {
        "NeoInfused",
        "GLFW",
        "stb",
        "InfluxRenderer"
    }

    filter "system:linux"
        toolset "gcc"
        defines { "NEO_PLATFORM_LINUX" }
        postbuildcommands {
            ("{COPY} ../bin/%{OUTPUT_DIR}/Influx-vk/libInflux-vk-bin.so ../bin/%{OUTPUT_DIR}/Nova/")
        }
    filter "system:windows"
        toolset "msc"
        defines {
            "NEO_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
        postbuildcommands {
            ("{COPY} %{INFLUX_VK_BIN}.dll ../bin/%{OUTPUT_DIR}/Nova/")
        }

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
        kind "WindowedApp"
        optimize "speed"
        symbols "off"
        runtime "Release"
        defines { "NEO_CONFIG_DIST" }
