project "ImGui"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_internal.h",
		"imgui_draw.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
        "imgui_tables.cpp"
	}

	

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

        defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter { "system:window", "configurations:Release" }
		--buildoptions "/MT"
	

