#pragma once

#define XINPUT_WRAPPER_NO_FUNCTIONS
#include <XInputWrapper.h>

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:XInputGetState=_XInputGetState@8")
#pragma comment(linker, "/EXPORT:_XInputDestroy=_XInputDestroy@0")
#pragma comment(linker, "/EXPORT:_XInputEnable=_XInputEnable@4")
#pragma comment(linker, "/EXPORT:_XInputGetBatteryInformation=_XInputGetBatteryInformation@12")
#pragma comment(linker, "/EXPORT:_XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids@12")
#pragma comment(linker, "/EXPORT:_XInputGetKeystroke=_XInputGetKeystroke@12")
#pragma comment(linker, "/EXPORT:_XInputInit=_XInputInit@0")
#pragma comment(linker, "/EXPORT:_XInputSetState=_XInputSetState@8")
#pragma comment(linker, "/EXPORT:_XInputSetStateEx=_XInputSetStateEx@8")
#pragma comment(linker, "/EXPORT:_XInputUpdate=_XInputUpdate@0")
#endif

#ifdef __cplusplus
extern "C" {
#endif
	#define DLL_EXPORT  __declspec( dllexport )

	DLL_EXPORT DWORD WINAPI		XInputInit();
	DLL_EXPORT void WINAPI		XInputDestroy();
	DLL_EXPORT void WINAPI		XInputUpdate();

	DLL_EXPORT DWORD WINAPI XInputGetState
	(
		__in  DWORD         dwUserIndex,  // Index of the gamer associated with the device
		__out XINPUT_STATE* pState        // Receives the current state
	);

	DLL_EXPORT DWORD WINAPI XInputSetState
	(
		__in DWORD             dwUserIndex,  // Index of the gamer associated with the device
		__in XINPUT_VIBRATION* pVibration    // The vibration information to send to the controller
	);

	DLL_EXPORT DWORD WINAPI XInputSetStateEx
	(
		__in DWORD               dwUserIndex,  // Index of the gamer associated with the device
		__in XINPUT_VIBRATION_EX* pVibration    // The vibration information to send to the controller
	);

	DLL_EXPORT DWORD WINAPI XInputGetCapabilities
	(
		__in  DWORD                dwUserIndex,   // Index of the gamer associated with the device
		__in  DWORD                dwFlags,       // Input flags that identify the device type
		__out XINPUT_CAPABILITIES* pCapabilities  // Receives the capabilities
	);

	DLL_EXPORT void WINAPI XInputEnable
	(
		__in BOOL enable     // [in] Indicates whether xinput is enabled or disabled. 
	);

	DLL_EXPORT DWORD WINAPI XInputGetDSoundAudioDeviceGuids
	(
		__in  DWORD dwUserIndex,          // Index of the gamer associated with the device
		__out GUID* pDSoundRenderGuid,    // DSound device ID for render
		__out GUID* pDSoundCaptureGuid    // DSound device ID for capture
	);

#ifndef XINPUT_USE_9_1_0

	DLL_EXPORT DWORD WINAPI XInputGetBatteryInformation
	(
		__in  DWORD                       dwUserIndex,        // Index of the gamer associated with the device
		__in  BYTE                        devType,            // Which device on this user index
		__out XINPUT_BATTERY_INFORMATION* pBatteryInformation // Contains the level and types of batteries
	);

	DLL_EXPORT DWORD WINAPI XInputGetKeystroke
	(
		__in       DWORD dwUserIndex,              // Index of the gamer associated with the device
		__reserved DWORD dwReserved,               // Reserved for future use
		__out      PXINPUT_KEYSTROKE pKeystroke    // Pointer to an XINPUT_KEYSTROKE structure that receives an input event.
	);

#endif //!XINPUT_USE_9_1_0

#ifdef __cplusplus
}
#endif

