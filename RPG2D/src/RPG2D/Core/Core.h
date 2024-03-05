#pragma once
#ifdef  RPG2D_PLATFORM_WINDOWS
	#ifdef RPG2D_BUILD_DLL
		#define RPG2D_API __declspec(dllexport)
	#else
		#define RPG2D_API __declspec(dllimport)
	#endif // DEBUG
#endif //  RPG2D_PLATFORM_WINDOWS