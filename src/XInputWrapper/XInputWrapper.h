#pragma once

#include <windef.h>

//
// Device types available in XINPUT_CAPABILITIES
//
#define XINPUT_DEVTYPE_GAMEPAD          0x01

//
// Device subtypes available in XINPUT_CAPABILITIES
//
#define XINPUT_DEVSUBTYPE_GAMEPAD       0x01

#ifndef XINPUT_USE_9_1_0

#define XINPUT_DEVSUBTYPE_WHEEL         0x02
#define XINPUT_DEVSUBTYPE_ARCADE_STICK  0x03
#define XINPUT_DEVSUBTYPE_FLIGHT_SICK   0x04
#define XINPUT_DEVSUBTYPE_DANCE_PAD     0x05
#define XINPUT_DEVSUBTYPE_GUITAR        0x06
#define XINPUT_DEVSUBTYPE_DRUM_KIT      0x08

#endif // !XINPUT_USE_9_1_0



//
// Flags for XINPUT_CAPABILITIES
//
#define XINPUT_CAPS_VOICE_SUPPORTED     0x0004
#define XINPUT_CAPS_WIRELESS			0x0008

//
// Constants for gamepad buttons
//
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000


//
// Gamepad thresholds
//
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

//
// Flags to pass to XInputGetCapabilities
//
#define XINPUT_FLAG_GAMEPAD             0x00000001


#ifndef XINPUT_USE_9_1_0

//
// Devices that support batteries
//
#define BATTERY_DEVTYPE_GAMEPAD         0x00
#define BATTERY_DEVTYPE_HEADSET         0x01

//
// Flags for battery status level
//
#define BATTERY_TYPE_DISCONNECTED       0x00    // This device is not connected
#define BATTERY_TYPE_WIRED              0x01    // Wired device, no battery
#define BATTERY_TYPE_ALKALINE           0x02    // Alkaline battery source
#define BATTERY_TYPE_NIMH               0x03    // Nickel Metal Hydride battery source
#define BATTERY_TYPE_UNKNOWN            0xFF    // Cannot determine the battery type

// These are only valid for wireless, connected devices, with known battery types
// The amount of use time remaining depends on the type of device.
#define BATTERY_LEVEL_EMPTY             0x00
#define BATTERY_LEVEL_LOW               0x01
#define BATTERY_LEVEL_MEDIUM            0x02
#define BATTERY_LEVEL_FULL              0x03

// User index definitions
#define XUSER_MAX_COUNT                 4

#define XUSER_INDEX_ANY                 0x000000FF


//
// Codes returned for the gamepad keystroke
//

#define VK_PAD_A                        0x5800
#define VK_PAD_B                        0x5801
#define VK_PAD_X                        0x5802
#define VK_PAD_Y                        0x5803
#define VK_PAD_RSHOULDER                0x5804
#define VK_PAD_LSHOULDER                0x5805
#define VK_PAD_LTRIGGER                 0x5806
#define VK_PAD_RTRIGGER                 0x5807

#define VK_PAD_DPAD_UP                  0x5810
#define VK_PAD_DPAD_DOWN                0x5811
#define VK_PAD_DPAD_LEFT                0x5812
#define VK_PAD_DPAD_RIGHT               0x5813
#define VK_PAD_START                    0x5814
#define VK_PAD_BACK                     0x5815
#define VK_PAD_LTHUMB_PRESS             0x5816
#define VK_PAD_RTHUMB_PRESS             0x5817

#define VK_PAD_LTHUMB_UP                0x5820
#define VK_PAD_LTHUMB_DOWN              0x5821
#define VK_PAD_LTHUMB_RIGHT             0x5822
#define VK_PAD_LTHUMB_LEFT              0x5823
#define VK_PAD_LTHUMB_UPLEFT            0x5824
#define VK_PAD_LTHUMB_UPRIGHT           0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT         0x5826
#define VK_PAD_LTHUMB_DOWNLEFT          0x5827

#define VK_PAD_RTHUMB_UP                0x5830
#define VK_PAD_RTHUMB_DOWN              0x5831
#define VK_PAD_RTHUMB_RIGHT             0x5832
#define VK_PAD_RTHUMB_LEFT              0x5833
#define VK_PAD_RTHUMB_UPLEFT            0x5834
#define VK_PAD_RTHUMB_UPRIGHT           0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT         0x5836
#define VK_PAD_RTHUMB_DOWNLEFT          0x5837

//
// Flags used in XINPUT_KEYSTROKE
//
#define XINPUT_KEYSTROKE_KEYDOWN        0x0001
#define XINPUT_KEYSTROKE_KEYUP          0x0002
#define XINPUT_KEYSTROKE_REPEAT         0x0004

#endif //!XINPUT_USE_9_1_0

//
// Structures used by XInput APIs
//
typedef struct _XINPUT_GAMEPAD
{
	WORD                                wButtons;
	BYTE                                bLeftTrigger;
	BYTE                                bRightTrigger;
	SHORT                               sThumbLX;
	SHORT                               sThumbLY;
	SHORT                               sThumbRX;
	SHORT                               sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

typedef struct _XINPUT_STATE
{
	DWORD                               dwPacketNumber;
	XINPUT_GAMEPAD                      Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

typedef struct _XINPUT_VIBRATION
{
	WORD                                wLeftMotorSpeed;
	WORD                                wRightMotorSpeed;
} XINPUT_VIBRATION, *PXINPUT_VIBRATION;

typedef struct _XINPUT_VIBRATION_EX
{
	WORD                                wLeftMotorSpeed;
	WORD                                wRightMotorSpeed;
	WORD                                wLeftTriggerSpeed;
	WORD                                wRightTriggerSpeed;
} XINPUT_VIBRATION_EX, *PXINPUT_VIBRATION_EX;

typedef struct _XINPUT_CAPABILITIES
{
	BYTE                                Type;
	BYTE                                SubType;
	WORD                                Flags;
	XINPUT_GAMEPAD                      Gamepad;
	XINPUT_VIBRATION                    Vibration;
} XINPUT_CAPABILITIES, *PXINPUT_CAPABILITIES;

#ifndef XINPUT_USE_9_1_0

typedef struct _XINPUT_BATTERY_INFORMATION
{
	BYTE BatteryType;
	BYTE BatteryLevel;
} XINPUT_BATTERY_INFORMATION, *PXINPUT_BATTERY_INFORMATION;

typedef struct _XINPUT_KEYSTROKE
{
	WORD    VirtualKey;
	WCHAR   Unicode;
	WORD    Flags;
	BYTE    UserIndex;
	BYTE    HidCode;
} XINPUT_KEYSTROKE, *PXINPUT_KEYSTROKE;

#endif // !XINPUT_USE_9_1_0

#ifndef XINPUT_WRAPPER_NO_FUNCTIONS

// Uncomment macro in case of function names confict
//#define XINPUT_WRAPPER_NAMESPACE XInputWrapper

#ifdef XINPUT_WRAPPER_NAMESPACE
namespace XINPUT_WRAPPER_NAMESPACE
{
#endif

typedef void ( WINAPI *XInputUpdateFunc)();
typedef void( WINAPI *XInputDestroyFunc)();

typedef DWORD ( WINAPI *XInputGetStateFunc)
(
    __in  DWORD         dwUserIndex,  // Index of the gamer associated with the device
    __out XINPUT_STATE* pState        // Receives the current state
);

typedef DWORD ( WINAPI *XInputSetStateFunc)
(
    __in DWORD             dwUserIndex,  // Index of the gamer associated with the device
    __in XINPUT_VIBRATION* pVibration    // The vibration information to send to the controller
);
typedef DWORD( WINAPI  *XInputSetStateExFunc )
(
	__in DWORD                dwUserIndex,  // Index of the gamer associated with the device
	__in XINPUT_VIBRATION_EX* pVibration    // The vibration information to send to the controller
);

typedef DWORD ( WINAPI *XInputGetCapabilitiesFunc)
(
    __in  DWORD                dwUserIndex,   // Index of the gamer associated with the device
    __in  DWORD                dwFlags,       // Input flags that identify the device type
    __out XINPUT_CAPABILITIES* pCapabilities  // Receives the capabilities
);

typedef void ( WINAPI *XInputEnableFunc )
(
    __in BOOL enable     // [in] Indicates whether xinput is enabled or disabled. 
);

typedef DWORD ( WINAPI *XInputGetDSoundAudioDeviceGuidsFunc)
(
    __in  DWORD dwUserIndex,          // Index of the gamer associated with the device
    __out GUID* pDSoundRenderGuid,    // DSound device ID for render
    __out GUID* pDSoundCaptureGuid    // DSound device ID for capture
);

#ifndef XINPUT_USE_9_1_0

typedef DWORD ( WINAPI *XInputGetBatteryInformationFunc)
(
    __in  DWORD                       dwUserIndex,        // Index of the gamer associated with the device
    __in  BYTE                        devType,            // Which device on this user index
    __out XINPUT_BATTERY_INFORMATION* pBatteryInformation // Contains the level and types of batteries
);

typedef DWORD ( WINAPI *XInputGetKeystrokeFunc)
(
    __in       DWORD dwUserIndex,              // Index of the gamer associated with the device
    __reserved DWORD dwReserved,               // Reserved for future use
    __out      PXINPUT_KEYSTROKE pKeystroke    // Pointer to an XINPUT_KEYSTROKE structure that receives an input event.
);

#endif //!XINPUT_USE_9_1_0

static XInputDestroyFunc					XInputDestroy					= NULL;
static XInputUpdateFunc						XInputUpdate					= NULL;
static XInputGetStateFunc					XInputGetState					= NULL;
static XInputSetStateFunc					XInputSetState					= NULL;
static XInputSetStateExFunc					XInputSetStateEx				= NULL;
static XInputGetCapabilitiesFunc			XInputGetCapabilities			= NULL;
static XInputEnableFunc						XInputEnable					= NULL;
static XInputGetDSoundAudioDeviceGuidsFunc	XInputGetDSoundAudioDeviceGuids	= NULL;
#ifndef XINPUT_USE_9_1_0
static XInputGetBatteryInformationFunc		XInputGetBatteryInformation		= NULL;
static XInputGetKeystrokeFunc				XInputGetKeystroke				= NULL;
#endif //!XINPUT_USE_9_1_0


///////////////////////////////////////////////////////////////////////////////////////
inline DWORD WINAPI _XInputFakeSetStateEx( __in DWORD dwUserIndex, __in XINPUT_VIBRATION_EX* pVibration )
{
	XINPUT_VIBRATION oVibration;
	oVibration.wLeftMotorSpeed = pVibration->wLeftMotorSpeed;
	oVibration.wRightMotorSpeed = pVibration->wRightMotorSpeed;
	return XInputSetState( dwUserIndex, &oVibration );
}
inline void WINAPI _XInputEmpyUpdate() {}
inline void WINAPI _XInputEmpyDestroy() {}

inline bool _XInputCanUseUWP()
{
	typedef LONG NTSTATUS, *PNTSTATUS;
	#define STATUS_SUCCESS (0x00000000)

	typedef NTSTATUS (WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

	{
		HMODULE hMod = ::GetModuleHandleW( L"ntdll.dll" );
		if( hMod )
		{
			RtlGetVersionPtr fxPtr = ( RtlGetVersionPtr )::GetProcAddress( hMod, "RtlGetVersion" );
			if( fxPtr != NULL )
			{
				RTL_OSVERSIONINFOW rovi = { 0 };
				rovi.dwOSVersionInfoSize = sizeof( rovi );
				if( fxPtr( &rovi ) == STATUS_SUCCESS )
				{
					return rovi.dwMajorVersion >= 10;
				}
			}
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////

inline void XInputInit()
{
	bool bIsUWP = _XInputCanUseUWP();
	HMODULE hXInput = NULL;
	if( bIsUWP )
	{
#ifdef _WIN64
		hXInput = LoadLibrary( "XInputUWP64.dll" );
#else
		hXInput = LoadLibrary( "XInputUWP.dll" );
#endif
	}

	if( hXInput == NULL )
	{
		hXInput = LoadLibrary( "xinput1_3.dll" );
		bIsUWP = false;
	}

	if( hXInput )
	{
		XInputGetState					= ( XInputGetStateFunc )GetProcAddress( hXInput, "XInputGetState" );
		XInputSetState					= ( XInputSetStateFunc )GetProcAddress( hXInput, "XInputSetState" );
		XInputGetCapabilities			= ( XInputGetCapabilitiesFunc )GetProcAddress( hXInput, "XInputGetCapabilities" );
		XInputEnable					= ( XInputEnableFunc )GetProcAddress( hXInput, "XInputEnable" );
		XInputGetDSoundAudioDeviceGuids	= ( XInputGetDSoundAudioDeviceGuidsFunc )GetProcAddress( hXInput, "XInputGetDSoundAudioDeviceGuids" );
#ifndef XINPUT_USE_9_1_0
		XInputGetBatteryInformation		= ( XInputGetBatteryInformationFunc )GetProcAddress( hXInput, "XInputGetBatteryInformation" );
		XInputGetKeystroke				= ( XInputGetKeystrokeFunc )GetProcAddress( hXInput, "XInputGetKeystroke" );
#endif	
		if( bIsUWP )
		{
			XInputDestroy				= ( XInputDestroyFunc )GetProcAddress( hXInput, "XInputDestroy" );
			XInputUpdate				= ( XInputUpdateFunc )GetProcAddress( hXInput, "XInputUpdate" );
			XInputSetStateEx			= ( XInputSetStateExFunc )GetProcAddress( hXInput, "XInputSetStateEx" );

			typedef DWORD( WINAPI *_XInputUWPInitFunc )();
			( ( _XInputUWPInitFunc )GetProcAddress( hXInput, "XInputInit" ) )();
		}
		else
		{
			XInputUpdate = _XInputEmpyUpdate;
			XInputDestroy = _XInputEmpyDestroy;
			XInputSetStateEx = _XInputFakeSetStateEx;
		}
	}
}

#ifdef XINPUT_WRAPPER_NAMESPACE
}
#endif

#endif