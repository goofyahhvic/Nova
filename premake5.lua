OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}"

workspace "NovaWorkspace"
    architecture "x64"
    configurations { "dbg", "rel", "dist" }

    targetdir "bin/%{OUTPUT_DIR}/%{prj.name}/"
    objdir "bin-int/%{OUTPUT_DIR}/%{prj.name}/"

include "NeoInfused/NeoInfused-Premake.lua"
include "Nova/Nova-Premake.lua"