workspace "RPG2D"--项目名称
	architecture "x64"--平台架构
	startproject "Sandbox"--启动项

	configurations -- 版本
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" --输出路径

--第三方路径
IncludeDir = {}

IncludeDir["GLFW"] = "%{wks.location}/RPG2D/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/RPG2D/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/RPG2D/vendor/ImGui"
IncludeDir["stb_image"] = "%{wks.location}/RPG2D/vendor/stb_image"
IncludeDir["glm"] = "%{wks.location}/RPG2D/vendor/glm"

--[[
IncludeDir["yaml_cpp"] = "%{wks.location}/RPG2D/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/RPG2D/vendor/Box2D/include"
IncludeDir["filewatch"] = "%{wks.location}/RPG2D/vendor/filewatch"
IncludeDir["ImGuizmo"] = "%{wks.location}/RPG2D/vendor/ImGuizmo"
IncludeDir["entt"] = "%{wks.location}/RPG2D/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/RPG2D/vendor/mono/include"
IncludeDir["shaderc"] = "%{wks.location}/RPG2D/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/RPG2D/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["msdfgen"] = "%{wks.location}/RPG2D/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/RPG2D/vendor/msdf-atlas-gen/msdf-atlas-gen"
]]

--添加子模组premake 文件
include "RPG2D/vendor/GLFW" -- GLFW脚本文件
include "RPG2D/vendor/Glad"
include "RPG2D/vendor/imgui"

project "RPG2D"--项目名称
	location "RPG2D"
	kind "SharedLib"--项目类型
	language "C++"--语言
	staticruntime "off"

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

		--"vendor/ImGuizmo/ImGuizmo.h",
		--"vendor/ImGuizmo/ImGuizmo.cpp"
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
	}
	links
	{
		"GLFW",
		"opengl32.lib",
		"Glad",
		"ImGui",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"RPG2D_PLATFORM_WINDOWS",
			"RPG2D_BUILD_DLL",
			"GLFW_INCLUDE_NONE",--消除glad的重复包含
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
		"%{IncludeDir.glm}",
	}

	links{
		"RPG2D"
	}


	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
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
