
local WindowsSDKVersion = "10.0.16299.0"


sln = solution	"XInputWrapper"
	location		( _ACTION)
	
	configurations 			{ "Debug", "Release" }
	language 				"C++"
	platforms 				{ "x32", "x64" }
	
	targetdir				( "../Bin" )
	objdir					( "../Temp/".._ACTION )
	defines					{ "WIN32_LEAN_AND_MEAN"	}
	
	defines					{ "_HAS_EXCEPTIONS=0" }

	linkoptions				{ "/DEBUG:FULL"  }

		
	configuration{ "Debug" }
		defines 			{ "_DEBUG" }
	configuration{ "Release" }
		defines 			{ "NDEBUG=1" }
		
	configuration{ "Debug" }
		targetsuffix		( "_d" )
	configuration{ "Release" }
		targetsuffix		( "_r" )
	configuration{ "x64", "Debug" }
		targetsuffix		( "64_d" )
	configuration{ "x64", "Release" }
		targetsuffix		( "64_r" )
		
	configuration {}
	
-------------------------------------------------------------
local name = "Sample"
project( name, "" )
	uuid				( os.uuid() )
	kind					"consoleApp"
	targetname				( name )
	
	files 					{	"../src/Sample/**", "../src/XInputWrapper/**" }
	vpaths 					{	["*"] = "../src/Sample/**",
								["*"] = "../src/XInputWrapper/**" }
	includedirs				{	"../src/XInputWrapper/" }
	
	flags 					{ "ExtraWarnings", "No64BitChecks","NoEditAndContinue", "NoExceptions", "NoRTTI", "Symbols" }
	configuration{ "Release " }
		flags 				{ "Optimize" }
	configuration{ "Debug" }
		buildoptions 		{ "/Ob1" }

	-- Reset the configuration flag
	configuration{}
	
-------------------------------------------------------------
name = "XInputUWP"
project( "XInputUWP", "" )
	uuid				( os.uuid() )

	kind					"SharedLib"
	windowstargetplatformversion ( WindowsSDKVersion )
	targetname				( name )
	
	files 					{	"../src/XInputUWP/**",
								"../src/XInputWrapper/**" }
	vpaths 					{	["*"] = "../src/XInputUWP/**",
								["*"] = "../src/XInputWrapper/**" }
	includedirs				{ "../src/XInputWrapper" }
	defines					{ "_USRDLL" }							
	
	buildoptions 			{ "/ZW" }
	buildoptions 			{ "/AI\"$(VSInstallDir)\\Common7\\IDE\\VC\\vcpackages\" /AI\"$(WindowsSdkDir)UnionMetadata\\"..WindowsSDKVersion.."\"" }
		
	flags 					{ "ExtraWarnings", "No64BitChecks","NoEditAndContinue", "NoExceptions", "NoRTTI", "Symbols" }
	configuration{ "Release" }
		flags 				{ "Optimize" }
	configuration{ "Debug" }
		buildoptions 		{ "/Ob1" }
		
	
	-- Reset the configuration flag
	configuration{}

