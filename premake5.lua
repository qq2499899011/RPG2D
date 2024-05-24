workspace "RPG2D"--��Ŀ����
	architecture "x64"--ƽ̨�ܹ�
	startproject "Sandbox"--������

	configurations -- �汾
	{
		"Debug",
		"Release",
		"Dist"
	}
	--ȫ�ֶ���
	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" --���·��

--������·��
IncludeDir = {}

IncludeDir["GLFW"] = "%{wks.location}/RPG2D/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/RPG2D/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/RPG2D/vendor/ImGui"
IncludeDir["stb_image"] = "%{wks.location}/RPG2D/vendor/stb_image"
IncludeDir["glm"] = "%{wks.location}/RPG2D/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/RPG2D/vendor/entt/include"
IncludeDir["Box2D"] = "%{wks.location}/RPG2D/vendor/Box2D/include"
IncludeDir["SoLoud"] = "%{wks.location}/RPG2D/vendor/SoLoud/include"

--�����ģ��premake �ļ�
include "RPG2D/vendor/GLFW" -- GLFW�ű��ļ�
include "RPG2D/vendor/Glad"
include "RPG2D/vendor/imgui"
include "RPG2D/vendor/Box2D"
include "RPG2D/vendor/SoLoud"

project "RPG2D"--��Ŀ����
	location "RPG2D"
	kind "StaticLib"--��Ŀ����
	language "C++"--����
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "RPG2Dpch.h"--Ԥ����ͷ�ļ�
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
			"GLFW_INCLUDE_NONE",--����glad���ظ�����
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
