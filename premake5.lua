--
-- @file premake5.lua
--

-- Workspace Settings
workspace "ProjectDG"

  -- Language and Standard
  language "C++"
  cppdialect "C++20"

  -- Build Configuration and Macros
  location "./generated"
  configurations { "Debug", "Release", "Distribute" }
  filter { "configurations:Debug" }
    defines { "DG_DEBUG" }
    symbols "On"
  filter { "configurations:Release" }
    defines { "DG_RELEASE" }
    optimize "On"
  filter { "configurations:Distribute" }
    defines { "DG_RELEASE", "DG_DISTRIBUTE" }
    optimize "On"
  filter {}

  -- System Macros
  filter { "system:linux" }
    defines { "DG_LINUX" }
  filter {}

-- Engine Library
project "dg-engine"

  -- Static Library
  kind "StaticLib"

  -- Build Files and Output
  location "./generated/dg-engine"
  targetdir "./build/bin/dg-engine/%{cfg.buildcfg}"
  objdir "./build/obj/dg-engine/%{cfg.buildcfg}"

  -- Precompiled Header
  pchheader "DG_Pch.hpp"
  pchsource "./projects/dg-engine/src/DG_Pch.cpp"

  -- Include Directories
  includedirs {
    "./vendor/glm/include",
    "./projects/dg-engine/include"
  }

  -- Source Files
  files {
    "./projects/dg-engine/include/DG/Events/*.hpp",
    "./projects/dg-engine/include/DG/Graphics/*.hpp",
    "./projects/dg-engine/include/DG/Math/*.hpp",
    "./projects/dg-engine/include/DG/Core/*.hpp",
    
    "./projects/dg-engine/src/DG/Events/*.cpp",
    "./projects/dg-engine/src/DG/Graphics/*.cpp",
    "./projects/dg-engine/src/DG/Math/*.cpp",
    "./projects/dg-engine/src/DG/Core/*.cpp"
  }
  
-- Studio Application
project "dg-studio"

  -- Console Application
  kind "ConsoleApp"

  -- Build Files and Output
  location "./generated/dg-studio"
  targetdir "./build/bin/dg-studio/%{cfg.buildcfg}"
  objdir "./build/obj/dg-studio/%{cfg.buildcfg}"

  -- Precompiled Header
  pchheader "DGStudio_Pch.hpp"
  pchsource "./projects/dg-studio/src/DGStudio_Pch.cpp"

  -- Include Directories
  includedirs {
    "./vendor/glm/include",
    "./projects/dg-engine/include",
    "./projects/dg-studio/include"
  }

  -- Project srcs
  files {
    "./projects/dg-studio/include/**.hpp",
    "./projects/dg-studio/src/**.cpp"
  }

  -- Link Libraries
  libdirs {
    "./build/bin/dg-engine/%{cfg.buildcfg}"
  }
  links {
    "dg-engine"
  }
  