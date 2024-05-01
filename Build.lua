-- premake5.lua
workspace "Pistone-FileCompression"
   architecture "x64"
   configurations { "Debug", "Release"}
   startproject "Pistone"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Pistone/Build-App.lua"