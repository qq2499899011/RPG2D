workspace "RPG2D"--项目名称
	architecture "x64"--平台架构
	startproject "Sandbox"--启动项

	configurations -- 版本
	{
		"Debug",
		"Release",
		"Dist"
	}
	--全局定义
	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" --输出路径

--第三方路径
IncludeDir = {}

IncludeDir["GLFW"] = "%{wks.location}/RPG2D/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/RPG2D/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/RPG2D/vendor/ImGui"
IncludeDir["stb_image"] = "%{wks.location}/RPG2D/vendor/stb_image"
IncludeDir["glm"] = "%{wks.location}/RPG2D/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/RPG2D/vendor/entt/include"
IncludeDir["Box2D"] = "%{wks.location}/RPG2D/vendor/Box2D/include"
IncludeDir["SoLoud"] = "%{wks.location}/RPG2D/vendor/SoLoud/include"

--添加子模组premake 文件
include "RPG2D/vendor/GLFW" -- GLFW脚本文件
include "RPG2D/vendor/Glad"
include "RPG2D/vendor/imgui"
include "RPG2D/vendor/Box2D"
include "RPG2D/vendor/SoLoud"

project "RPG2D"--项目名称
	location "RPG2D"
	kind "StaticLib"--项目类型
	language "C++"--语言
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "RPG2Dpch.h"--预编译头文件
	pchsource "RPG2D/src/RPG2Dpch.cpp"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include", --spdlog
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.SoLoud}",
	}
	links
	{
		"GLFW",
		"opengl32.lib",
		"Glad",
		"ImGui",
		"Box2D",
		"SoLoud",
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"RPG2D_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",--消除glad的重复包含
		}

	filter "configurations:Debug"
		defines "RPG2D_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RPG2D_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RPG2D_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"RPG2D/vendor/spdlog/include",
		"RPG2D/src",
		"RPG2D/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"

	}

	links{
		"RPG2D"
	}


	filter "system:windows"
		systemversion "latest"
		defines
		{
			"RPG2D_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RPG2D_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RPG2D_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RPG2D_DIST"
		runtime "Release"
		optimize "On"
