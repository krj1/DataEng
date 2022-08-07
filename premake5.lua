-- premake5.lua
workspace "DataEng"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "DataEng"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "DataEng"