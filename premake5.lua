--workspace: 对应VS中的解决方案
workspace "TheRayTracerChallenge"
    configurations { "Debug", "Release" }    --解决方案配置项，Debug和Release默认配置
    location "."                             --解决方案文件夹，这是我自己喜欢用的project简写
    cppdialect "c++17"    
    startproject "TheRayTracerChallenge"
    -- Turn on DPI awareness
    dpiawareness "High"

    --增加平台配置，我希望有Win32和x64两种平台
    platforms
    {
        "Win32",
        "x64"
    }

    --Win32平台配置属性
    filter "platforms:Win32"
        architecture "x86"      --指定架构为x86
        targetdir ("bin/%{cfg.buildcfg}_%{cfg.platform}/")      --指定输出目录
        objdir  ("obj/%{cfg.buildcfg}_%{cfg.platform}/")        --指定中间目录

    --x64平台配置属性
    filter "platforms:x64"
        architecture "x64"      --指定架构为x64
        targetdir ("bin/%{cfg.buildcfg}_%{cfg.platform}/")      --指定输出目录
        objdir  ("obj/%{cfg.buildcfg}_%{cfg.platform}/")        --指定中间目录

--project: 对应VS中的项目
project "TheRayTracerChallenge"
    kind "ConsoleApp"                       --项目类型，控制台程序
    language "C++"                          --工程采用的语言，Premake5.0当前支持C、C++、C#
    location "Project"
    openmp "On"
    postbuildcommands 
    {
        "%{wks.location}/bin/%{cfg.buildcfg}_%{cfg.platform}/%{prj.name}.exe"
    }

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }                                       --指定加载哪些文件或哪些类型的文件
    
    excludes 
    { 
        "src/Tests/**.h", 
        "src/Tests/**.cpp", 
    }

    excludes 
    { 
        "src/aa/**.h", 
        "src/aa/**.cpp", 
    }
    -- -- Exclude template files
    -- filter { "files:**features.cpp" }
    --     -- buildaction("None")
    --     flags {"ExcludeFromBuild"}

    -- vpaths 
    -- {
    --     -- ["Headers/*"] = { "*.h", "*.hpp" },  --包含具体路径
    --     -- ["Sources/*"] = { "*.c", "*.cpp" },  --包含具体路径
    --     ["Headers"] = { "**.h", "**.hpp" },     --不包含具体路径
    --     ["Sources"] = { "**.c", "**.cpp" },     --不包含具体路径
    --     ["Docs"] = "**.md"
    -- }

    --Debug配置项属性
    filter "configurations:Debug"
        defines { "DEBUG", "ARIA_DEBUG", "ARIA_PLATFORM_WINDOWS" }                 --定义Debug宏(这可以算是默认配置)
        symbols "On"                                           --开启调试符号
        debugdir "%{wks.location}"

        includedirs 
        { 
            './ThirdParty/Catch2/include',
            './src'
        }

		libdirs 
        { 
            './ThirdParty/Catch2/lib'
        }

		links 
        {
            'Catch2d.lib',
            'Catch2Maind.lib'
        }

    --Release配置项属性
    filter "configurations:Release"
        defines { "NDEBUG", "ARIA_RELEASE", "ARIA_PLATFORM_WINDOWS" }                 --定义NDebug宏(这可以算是默认配置)
        optimize "On"                                           --开启优化参数
        debugdir "%{wks.location}"

        includedirs 
        { 
            './ThirdParty/Catch2/include',
            './src'
        }

		libdirs 
        { 
            './ThirdParty/Catch2/lib'
        }

		links 
        { 
            'Catch2.lib',
            'Catch2Main.lib'
        }

--project: 对应VS中的项目
project "TestCases"
    kind "ConsoleApp"                       --项目类型，控制台程序
    language "C++"                          --工程采用的语言，Premake5.0当前支持C、C++、C#
    location "Project"
    openmp "On"

    files 
    { 
        "src/Tests/**.h", 
        "src/Tests/**.cpp",
        "src/canvas.cpp",
        "src/intersection.cpp",
    }                                       --指定加载哪些文件或哪些类型的文件

    -- Exclude template files
    filter { "files:**.features.cpp" }
        -- buildaction("None")
        flags {"ExcludeFromBuild"}

    -- vpaths 
    -- {
    --     -- ["Headers/*"] = { "*.h", "*.hpp" },  --包含具体路径
    --     -- ["Sources/*"] = { "*.c", "*.cpp" },  --包含具体路径
    --     ["Headers"] = { "**.h", "**.hpp" },     --不包含具体路径
    --     ["Sources"] = { "**.c", "**.cpp" },     --不包含具体路径
    --     ["Docs"] = "**.md"
    -- }

    --Debug配置项属性
    filter "configurations:Debug"
        defines { "DEBUG", "ARIA_DEBUG", "ARIA_PLATFORM_WINDOWS" }                 --定义Debug宏(这可以算是默认配置)
        symbols "On"                                           --开启调试符号
        debugdir "%{wks.location}"
        debugargs "--reporter compact --success"

        includedirs 
        { 
            './ThirdParty/Catch2/include',
            './src'
        }

		libdirs 
        { 
            './ThirdParty/Catch2/lib'
        }

		links 
        {
            'Catch2d.lib',
            'Catch2Maind.lib'
        }

    --Release配置项属性
    filter "configurations:Release"
        defines { "NDEBUG", "ARIA_RELEASE", "ARIA_PLATFORM_WINDOWS" }                 --定义NDebug宏(这可以算是默认配置)
        optimize "On"                                           --开启优化参数
        debugdir "%{wks.location}"
        debugargs "--reporter compact --success"

        includedirs 
        { 
            './ThirdParty/Catch2/include',
            './src'
        }

		libdirs 
        { 
            './ThirdParty/Catch2/lib'
        }

		links 
        { 
            'Catch2.lib',
            'Catch2Main.lib'
        }