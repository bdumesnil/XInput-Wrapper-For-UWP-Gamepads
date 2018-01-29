#pragma once

#define XINPUT_WRAPPER_NO_FUNCTIONS
#include <XInputWrapper.h>

#ifdef __cplusplus
extern "C" {
#endif
	#define WRAPPER_API __declspec( dllexport )

	DWORD WRAPPER_API		XInputInit();
	void WRAPPER_API		XInputDestroy();
	void WRAPPER_API		XInputUpdate();

	DWORD WRAPPER_API XInputGetState
	(
		__in  DWORD         dwUserIndex,  // Index of the gamer associated with the device
		__out XINPUT_STATE* pState        // Receives the current state
	);

	DWORD WRAPPER_API XInputSetState
	(
		__in DWORD             dwUserIndex,  // Index of the gamer associated with the device
		__in XINPUT_VIBRATION* pVibration    // The vibration information to send to the controller
	);

	DWORD WRAPPER_API XInputSetStateEx
	(
		__in DWORD               dwUserIndex,  // Index of the gamer associated with the device
		__in XINPUT_VIBRATION_EX* pVibration    // The vibration information to send to the controller
	);

	DWORD WRAPPER_API XInputGetCapabilities
	(
		__in  DWORD                dwUserIndex,   // Index of the gamer associated with the device
		__in  DWORD                dwFlags,       // Input flags that identify the device type
		__out XINPUT_CAPABILITIES* pCapabilities  // Receives the capabilities
	);

	void WRAPPER_API XInputEnable
	(
		__in BOOL enable     // [in] Indicates whether xinput is enabled or disabled. 
	);

	DWORD WRAPPER_API XInputGetDSoundAudioDeviceGuids
	(
		__in  DWORD dwUserIndex,          // Index of the gamer associated with the device
		__out GUID* pDSoundRenderGuid,    // DSound device ID for render
		__out GUID* pDSoundCaptureGuid    // DSound device ID for capture
	);

#ifndef XINPUT_USE_9_1_0

	DWORD WRAPPER_API XInputGetBatteryInformation
	(
		__in  DWORD                       dwUserIndex,        // Index of the gamer associated with the device
		__in  BYTE                        devType,            // Which device on this user index
		__out XINPUT_BATTERY_INFORMATION* pBatteryInformation // Contains the level and types of batteries
	);

	DWORD WRAPPER_API XInputGetKeystroke
	(
		__in       DWORD dwUserIndex,              // Index of the gamer associated with the device
		__reserved DWORD dwReserved,               // Reserved for future use
		__out      PXINPUT_KEYSTROKE pKeystroke    // Pointer to an XINPUT_KEYSTROKE structure that receives an input event.
	);

#endif //!XINPUT_USE_9_1_0

#ifdef __cplusplus
}
#endif

