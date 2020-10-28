newoption
{
	trigger = "vsversion",
	description = "Specify the Visual Studio version",
	default = "2019",
	allowed = {
		{ "2019", "Visual Studio 2019"},
		{ "2017", "Visual Studio 2017"}
	}
}

newoption
{
	trigger = "vspath",
	description = "Path to Visual Studio",
	default = 'C:\\"Program Files (x86)"\\"Microsoft Visual Studio"\\'
}

newoption
{
	trigger = "vstype",
	description = "Community / Enterprise / Professional",
	allowed = {
		{"Community", "Visual Studio Community"},
		{"Enterprise", "Visual Studio Enterprise"},
		{"Professional", "Visual Studio Professional"}
	}
}

newoption
{
	trigger = "buildconfig",
	description = "Specify the build configurations",
	default = "Both",
	allowed = {
		{"Debug", "Debug"},
		{"Release", "Release"},
		{"Both", "Release and Debug"}
	}
}

newoption
{
	trigger = "buildplatform",
	description = "Specify the build platform",
	default = "Both",
	allowed = {
		{"x86", "x86"},
		{"x64", "x64"},
		{"Both", "x86 and x64"}
	}
}

local BuildSettings = 
{
	vsversion = _OPTIONS["vsversion"] or "2019",
	vspath = _OPTIONS["vspath"] or 'C:\\"Program Files (x86)"\\"Microsoft Visual Studio"\\',
	vstype = _OPTIONS["vstype"] or 'Enterprise',
	buildconfigs = {},
	buildplatforms = {}
}

if _OPTIONS["buildplatform"] == "Both" then
	BuildSettings.buildplatforms = {"x86", "x64"} else
	BuildSettings.buildplatforms = {_OPTIONS["buildplatform"]} end
	
if _OPTIONS["buildconfig"] == "Both" then
	BuildSettings.buildconfigs = {"Release", "Debug"} else
	BuildSettings.buildconfigs = {_OPTIONS["buildconfig"]} end

newaction {
	trigger = "build",
	description = "Build the project",
	execute = function()
		local initVS = BuildSettings.vspath .. BuildSettings.vsversion .. '\\' .. BuildSettings.vstype .. '\\VC\\Auxiliary\\Build\\vcvars32.bat'
		local buildCommandTemplate = 'msbuild "GAP321_KirillSizykh.sln" /p:Platform=%s /p:Configuration=%s /m'
		local buildCommand = ""
		
		for k,config in ipairs(BuildSettings.buildconfigs) do
			for j, platform in ipairs(BuildSettings.buildplatforms) do
				buildCommand = buildCommand .. string.format(buildCommandTemplate, platform, config) .. " && "
			end
		end
		
		buildCommand = buildCommand:sub(1, #buildCommand - 4)
		os.execute('premake5 vs' .. BuildSettings.vsversion .. ' && ' .. initVS .. ' && ' .. buildCommand)
	end
}

workspace "GAP391_KirillSizykh"
	startproject "CellularAutomata"
	configurations {"Debug", "Release"}
	platforms {"x86", "x64"}
	language "C++"
	cppdialect "C++17"
	
project "Engine"
	kind "StaticLib"
	location "Yangine"
	includedirs {"Yangine/Source", "Toolset/Include"}
	files {"Yangine/Source/**.h", "Yangine/Source/**.cpp"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
	
project "PerlinNoiseWorld"
	kind "ConsoleApp"
	location "PerlinNoiseWorld"
	includedirs {"Yangine/Source", "Toolset/Include", "PerlinNoiseWorld/Source"}
	files {"PerlinNoiseWorld/Source/**.h", "PerlinNoiseWorld/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "PerlinNoiseWorld/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "PerlinNoiseWorld/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "PerlinNoiseWorld/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "PerlinNoiseWorld/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
		
	
project "CellularAutomata"
	kind "ConsoleApp"
	location "CellularAutomata"
	includedirs {"Yangine/Source", "Toolset/Include", "CellularAutomata/Source"}
	files {"CellularAutomata/Source/**.h", "CellularAutomata/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "CellularAutomata/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "CellularAutomata/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "CellularAutomata/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "CellularAutomata/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
		
	
project "GameOfLife"
	kind "ConsoleApp"
	location "GameOfLife"
	includedirs {"Yangine/Source", "Toolset/Include", "GameOfLife/Source"}
	files {"GameOfLife/Source/**.h", "GameOfLife/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "GameOfLife/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "GameOfLife/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "GameOfLife/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "GameOfLife/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
		
	
project "CellularAutomataWorld"
	kind "ConsoleApp"
	location "CellularAutomataWorld"
	includedirs {"Yangine/Source", "Toolset/Include", "CellularAutomataWorld/Source"}
	files {"CellularAutomataWorld/Source/**.h", "CellularAutomataWorld/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "CellularAutomataWorld/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "CellularAutomataWorld/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "CellularAutomataWorld/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "CellularAutomataWorld/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
		
	
project "LSystems"
	kind "ConsoleApp"
	location "LSystems"
	includedirs {"Yangine/Source", "Toolset/Include", "LSystems/Source"}
	files {"LSystems/Source/**.h", "LSystems/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "LSystems/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "LSystems/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "LSystems/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "LSystems/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
	
project "CustomGrammar"
	kind "ConsoleApp"
	location "CustomGrammar"
	includedirs {"Yangine/Source", "Toolset/Include", "CustomGrammar/Source"}
	files {"CustomGrammar/Source/**.h", "CustomGrammar/Source/**.cpp"}
	links {"Engine", "vld", "Box2D_$(PlatformShortName)_$(Configuration)", "SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "SDL2main", "Lua-5.3.5_$(PlatformShortName)_$(Configuration)"}
	
	postbuildcommands { 'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.dll" "$(OutDir)" /d /i /y',
	'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.manifest" "$(OutDir)" /d /i /y',
	'xcopy "$(ProjectDir)\\Assets" "$(OutDir)Assets" /e /s /h /i /y',
	'xcopy "$(ProjectDir)\\Data" "$(OutDir)Data" /e /s /h /i /y'}
	
	filter {"platforms:x86"}
		libdirs{"Toolset/x86"}
		
	filter {"platforms:x64"}
		libdirs{"Toolset/x64"}
	
	filter {"configurations:Debug", "platforms:x86"}
		architecture "x86"
		targetdir "CustomGrammar/Builds/Debug_x86"
		libdirs "Yangine/Binaries/Debug_x86"
		
	filter {"configurations:Release", "platforms:x86"}
		architecture "x86"
		targetdir "CustomGrammar/Builds/Release_x86"
		libdirs "Yangine/Binaries/Release_x86"
		
	filter {"configurations:Debug", "platforms:x64"}
		architecture "x86_64"
		targetdir "CustomGrammar/Builds/Debug_x64"
		libdirs "Yangine/Binaries/Debug_x64"
		
	filter {"configurations:Release", "platforms:x64"}
		architecture "x86_64"
		targetdir "CustomGrammar/Builds/Release_x64"
		libdirs "Yangine/Binaries/Release_x64"
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		postbuildcommands {'xcopy "$(SolutionDir)Toolset\\$(PlatformShortName)\\*.pdb" "$(OutDir)" /d /i /y'}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
		
	