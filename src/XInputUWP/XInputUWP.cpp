
#include <ctime>
#include <atomic>
#include <wrl/client.h>
#include <wrl/event.h>
#include <wrl/wrappers/corewrappers.h>
#include <windows.gaming.input.h>
#include <Windows.Gaming.UI.h>
#include <windows.ui.h>
#include <windows.ui.core.h>
#include <winerror.h>

#include "XInputUWP.h"
#include <shlobj.h>

using namespace ABI::Windows::Gaming::Input;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
namespace gameui = ABI::Windows::Gaming::UI;
namespace WRL = Microsoft::WRL;
namespace awf = ABI::Windows::Foundation;

#ifdef __cplusplus
extern "C" {
#endif

struct XInputState
{
	IGamepad*				pGamepads[ XUSER_MAX_COUNT ];
	UINT64					iTimestamps[ XUSER_MAX_COUNT ];
	XINPUT_STATE			oLastStates[ XUSER_MAX_COUNT ];
	Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepadStatics> oGamePadStatics;
	EventRegistrationToken	oOnControllerAddedToken;
	EventRegistrationToken	oOnControllerRemovedToken;
	bool					bIsEnabled;
	CRITICAL_SECTION		oCriticalSection;
};
static XInputState g_oState;

struct Lock
{
	Lock( CRITICAL_SECTION* pLock ) : m_pLock( pLock )
	{
		EnterCriticalSection( m_pLock );
	}
	~Lock()
	{
		LeaveCriticalSection( m_pLock );
	}
	CRITICAL_SECTION* m_pLock;
};


HRESULT OnGamepadAdded( IInspectable*, IGamepad* pGamepad )
{
	Lock oLock( &g_oState.oCriticalSection );

	for( int i = 0; i < XUSER_MAX_COUNT; ++i )
	{
		if( g_oState.pGamepads[ i ] == NULL )
		{
			g_oState.pGamepads[ i ] = pGamepad;
			break;
		}
	}
	return S_OK;
}

HRESULT OnGamepadRemoved( IInspectable*, IGamepad* pGamepad )
{
	Lock oLock( &g_oState.oCriticalSection );

	for( int i = 0; i < XUSER_MAX_COUNT; ++i )
	{
		if( g_oState.pGamepads[ i ] == pGamepad )
		{
			g_oState.pGamepads[ i ] = NULL;
			break;
		}
	}
	return S_OK;
}
auto g_oGamepadAddedHandler = WRL::Callback<awf::IEventHandler<Gamepad*>>( OnGamepadAdded );
auto g_oGamepadRemovedHandler = WRL::Callback<awf::IEventHandler<Gamepad*>>( OnGamepadRemoved );

DWORD XInputInit()
{
	HRESULT hr = S_OK;
	hr = Windows::Foundation::Initialize( RO_INIT_MULTITHREADED );
	if( FAILED( hr ) )
		return hr;
	InitializeCriticalSection( &g_oState.oCriticalSection );

	for( int i = 0; i < XUSER_MAX_COUNT; ++i )
	{
		g_oState.pGamepads[ i ]		= NULL;
		g_oState.iTimestamps[ i ]	= 0;
		g_oState.oLastStates[ i ]	= { 0 };
	}
	g_oState.oOnControllerAddedToken = { 0 };
	g_oState.oOnControllerRemovedToken = { 0 };
	g_oState.bIsEnabled = true;

	Lock oLock( &g_oState.oCriticalSection );

	hr = Windows::Foundation::GetActivationFactory( Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_Gaming_Input_Gamepad ).Get(), &g_oState.oGamePadStatics );
	if( FAILED( hr ) )
		return hr;
	hr = g_oState.oGamePadStatics->add_GamepadAdded( g_oGamepadAddedHandler.Get(), &g_oState.oOnControllerAddedToken );
	if( FAILED( hr ) )
		return hr;
	hr = g_oState.oGamePadStatics->add_GamepadRemoved( g_oGamepadRemovedHandler.Get(), &g_oState.oOnControllerRemovedToken );
	if( FAILED( hr ) )
		return hr;

	Microsoft::WRL::ComPtr<IVectorView<Gamepad*>> pGamepads = NULL;
	hr = g_oState.oGamePadStatics->get_Gamepads( pGamepads.GetAddressOf() );
	if( SUCCEEDED( hr ) )
	{
		unsigned iCount = 0;
		pGamepads->get_Size( &iCount );
		for( unsigned iGamepad = 0; iGamepad < iCount && iGamepad < XUSER_MAX_COUNT; ++iGamepad )
		{
			IGamepad* pGamepad = NULL;
			hr = pGamepads->GetAt( iGamepad, &pGamepad );
			if( SUCCEEDED( hr ) && pGamepad != NULL )
			{
				g_oState.pGamepads[ iGamepad ] = pGamepad;
			}
		}
	}	
	return hr;
}

void XInputUpdate()
{
	Lock oLock( &g_oState.oCriticalSection );
	Microsoft::WRL::ComPtr<IVectorView<Gamepad*>> pGamepads = NULL;
	HRESULT hr = g_oState.oGamePadStatics->get_Gamepads( pGamepads.GetAddressOf() );
	if( SUCCEEDED( hr ) )
	{
		unsigned iConnectedGamepads = 0;
		pGamepads->get_Size( &iConnectedGamepads );

		// Check if all bound gamepads are still connected
		for( int iBoundGamePad = 0; iBoundGamePad < XUSER_MAX_COUNT; ++iBoundGamePad )
		{
			if( g_oState.pGamepads[ iBoundGamePad ]== NULL )
				continue;
			bool bStillConnected = false;
			for( unsigned iConnectedGamePad = 0; iConnectedGamePad < iConnectedGamepads; ++iConnectedGamePad )
			{
				IGamepad* pGamepad = NULL;
				hr = pGamepads->GetAt( iConnectedGamePad, &pGamepad );
				if( pGamepad == g_oState.pGamepads[ iBoundGamePad ] )
				{
					bStillConnected = true;
					break;
				}
			}
			if( bStillConnected == false )
				g_oState.pGamepads[ iBoundGamePad ] = NULL;
		}
		// Check if all connected gamepads are already bound
		for( unsigned iConnectedGamePad = 0; iConnectedGamePad < iConnectedGamepads; ++iConnectedGamePad )
		{
			Microsoft::WRL::ComPtr<IGamepad> pGamepad = NULL;
			hr = pGamepads->GetAt( iConnectedGamePad, pGamepad.GetAddressOf() );
			int iFirstFreeGamepad = -1;
			bool bAlreadyBound = false;
			for( int iBoundGamePad = 0; iBoundGamePad < XUSER_MAX_COUNT; ++iBoundGamePad )
			{
				if( iFirstFreeGamepad == -1 && g_oState.pGamepads[ iConnectedGamePad ] == NULL )
					iFirstFreeGamepad = iBoundGamePad;
				if( pGamepad.Get() == g_oState.pGamepads[ iBoundGamePad ] )
				{
					bAlreadyBound = true;
					break;
				}
			}
			if( bAlreadyBound == false )
				g_oState.pGamepads[ iFirstFreeGamepad ] = pGamepad.Get();
		}
	}
}

void XInputDestroy()
{
	EnterCriticalSection( &g_oState.oCriticalSection );

	HRESULT hr = g_oState.oGamePadStatics->remove_GamepadAdded( g_oState.oOnControllerAddedToken );
	hr = g_oState.oGamePadStatics->remove_GamepadRemoved( g_oState.oOnControllerRemovedToken );
	DeleteCriticalSection( &g_oState.oCriticalSection );
	g_oState.oGamePadStatics.Reset();
}

#define MAX_SHORT 32767.0

static unsigned int g_iUWPToXInput[]
{
	XINPUT_GAMEPAD_START,			// GamepadButtons_Menu = 0x1,
	XINPUT_GAMEPAD_BACK,			// GamepadButtons_View = 0x2,
	XINPUT_GAMEPAD_A,				// GamepadButtons_A = 0x4,
	XINPUT_GAMEPAD_B,				// GamepadButtons_B = 0x8,
	XINPUT_GAMEPAD_X,				// GamepadButtons_X = 0x10,
	XINPUT_GAMEPAD_Y,				// GamepadButtons_Y = 0x20,
	XINPUT_GAMEPAD_DPAD_UP,			// GamepadButtons_DPadUp = 0x40,
	XINPUT_GAMEPAD_DPAD_DOWN,		// GamepadButtons_DPadDown = 0x80,
	XINPUT_GAMEPAD_DPAD_LEFT,		// GamepadButtons_DPadLeft = 0x100,
	XINPUT_GAMEPAD_DPAD_RIGHT,		// GamepadButtons_DPadRight = 0x200,
	XINPUT_GAMEPAD_LEFT_SHOULDER,	// GamepadButtons_LeftShoulder = 0x400,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,	// GamepadButtons_RightShoulder = 0x800,
	XINPUT_GAMEPAD_LEFT_THUMB,		// GamepadButtons_LeftThumbstick = 0x1000,
	XINPUT_GAMEPAD_RIGHT_THUMB,		// GamepadButtons_RightThumbstick = 0x2000,
	0,								// GamepadButtons_Paddle1 = 0x4000,
	0,								// GamepadButtons_Paddle2 = 0x8000,
	0,								// GamepadButtons_Paddle3 = 0x10000,
	0,								// GamepadButtons_Paddle4 = 0x20000,
};

DWORD XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pState == NULL )
		return ERROR_INVALID_DATA;

	IGamepad* pGamepad = g_oState.pGamepads[ dwUserIndex ];
	ABI::Windows::Gaming::Input::GamepadReading oReading;
	HRESULT hr = pGamepad->GetCurrentReading( &oReading );
	if( SUCCEEDED( hr ) == FALSE )
		return hr;

	if( g_oState.iTimestamps[ dwUserIndex ] == oReading.Timestamp )
	{
		*pState = g_oState.oLastStates[ dwUserIndex ];
		return ERROR_SUCCESS;
	}
	pState->Gamepad = { 0 };
	if( g_oState.bIsEnabled )
	{
		pState->Gamepad.sThumbLX		= ( SHORT )( oReading.LeftThumbstickX * MAX_SHORT );
		pState->Gamepad.sThumbLY		= ( SHORT )( oReading.LeftThumbstickY * MAX_SHORT );
		pState->Gamepad.sThumbRX		= ( SHORT )( oReading.RightThumbstickX * MAX_SHORT );
		pState->Gamepad.sThumbRY		= ( SHORT )( oReading.RightThumbstickY * MAX_SHORT );
		pState->Gamepad.bLeftTrigger	= ( BYTE )( oReading.LeftTrigger * 255.0 );
		pState->Gamepad.bRightTrigger	= ( BYTE )( oReading.RightTrigger * 255.0 );
	
		unsigned int iButtons = oReading.Buttons;
		while( iButtons != 0 )
		{
			unsigned int long iIndex = 0;
			_BitScanForward( &iIndex, iButtons );
			iButtons &= ~( 1 << iIndex );
			pState->Gamepad.wButtons |= g_iUWPToXInput[ iIndex ];
		}
	}
	pState->dwPacketNumber = g_oState.oLastStates[ dwUserIndex ].dwPacketNumber + 1;
	g_oState.oLastStates[ dwUserIndex ] = *pState;
	g_oState.iTimestamps[ dwUserIndex ] = oReading.Timestamp;

	return ERROR_SUCCESS;
}

DWORD XInputSetState( __in DWORD dwUserIndex, __in XINPUT_VIBRATION* pVibration )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pVibration == NULL )
		return ERROR_INVALID_DATA;

	IGamepad* pGamepad = g_oState.pGamepads[ dwUserIndex ];
	ABI::Windows::Gaming::Input::GamepadVibration oValue = { 0 };
	if( g_oState.bIsEnabled )
	{
		oValue.LeftMotor	= ( DOUBLE )pVibration->wLeftMotorSpeed / 65535.0;
		oValue.RightMotor	= ( DOUBLE )pVibration->wRightMotorSpeed / 65535.0;
	}
	if( SUCCEEDED( pGamepad->put_Vibration( oValue ) ) == FALSE )
		return ERROR_INVALID_DATA;
	return ERROR_SUCCESS;
}

DWORD XInputSetStateEx( __in DWORD dwUserIndex, __in XINPUT_VIBRATION_EX* pVibration )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pVibration == NULL )
		return ERROR_INVALID_DATA;

	IGamepad* pGamepad = g_oState.pGamepads[ dwUserIndex ];
	ABI::Windows::Gaming::Input::GamepadVibration oValue = { 0 };
	if( g_oState.bIsEnabled )
	{
		oValue.LeftMotor	= ( DOUBLE )pVibration->wLeftMotorSpeed / 65535.0;
		oValue.RightMotor	= ( DOUBLE )pVibration->wRightMotorSpeed / 65535.0;
		oValue.LeftTrigger	= ( DOUBLE )pVibration->wLeftTriggerSpeed / 65535.0;
		oValue.RightTrigger	= ( DOUBLE )pVibration->wRightTriggerSpeed / 65535.0;
	}
	HRESULT hr = pGamepad->put_Vibration( oValue );
	return hr;
}

DWORD XInputGetCapabilities( __in DWORD dwUserIndex, __in DWORD dwFlags, __out XINPUT_CAPABILITIES* pCapabilities )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pCapabilities == NULL )
		return ERROR_INVALID_DATA;
	if( dwFlags != 0 && dwFlags != XINPUT_DEVTYPE_GAMEPAD )
	{
		__debugbreak();
		return ERROR_INVALID_DATA;
	}
	IGameController* pGameController = NULL;
	g_oState.pGamepads[ dwUserIndex ]->QueryInterface( &pGameController );
	pCapabilities->Type = XINPUT_DEVTYPE_GAMEPAD;
	pCapabilities->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;
	pCapabilities->Flags = 0;
	boolean bIsWireless = false;
	HRESULT hr = pGameController->get_IsWireless( &bIsWireless );
	if( SUCCEEDED( hr ) && bIsWireless )
		pCapabilities->Flags |= XINPUT_CAPS_WIRELESS;
	memset( &pCapabilities->Gamepad, 0xFFFFFFFF, sizeof( pCapabilities->Gamepad ) );
	memset( &pCapabilities->Vibration, 0xFFFFFFFF, sizeof( pCapabilities->Vibration ) );
	return ERROR_SUCCESS;
}

void XInputEnable( __in BOOL enable )
{
	Lock oLock( &g_oState.oCriticalSection );
	g_oState.bIsEnabled = ( enable == TRUE );
}

DWORD XInputGetDSoundAudioDeviceGuids( __in  DWORD dwUserIndex, __out GUID* pDSoundRenderGuid, __out GUID* pDSoundCaptureGuid )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pDSoundRenderGuid == NULL || pDSoundCaptureGuid == NULL )
		return ERROR_INVALID_DATA;
	*pDSoundRenderGuid = GUID_NULL;
	*pDSoundCaptureGuid = GUID_NULL;
	return ERROR_SUCCESS;
}

DWORD XInputGetBatteryInformation( __in  DWORD dwUserIndex, __in BYTE /*devType*/, __out XINPUT_BATTERY_INFORMATION* pBatteryInformation )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pBatteryInformation == NULL )
		return ERROR_INVALID_DATA;
	pBatteryInformation->BatteryLevel	= BATTERY_LEVEL_FULL;
	pBatteryInformation->BatteryType	= BATTERY_TYPE_UNKNOWN;
	return ERROR_SUCCESS;
}

DWORD XInputGetKeystroke( __in DWORD dwUserIndex, __reserved DWORD /*dwReserved*/, __out PXINPUT_KEYSTROKE pKeystroke )
{
	Lock oLock( &g_oState.oCriticalSection );

	if( ( dwUserIndex >= XUSER_MAX_COUNT ) || ( g_oState.pGamepads[ dwUserIndex ] == NULL ) )
		return ERROR_DEVICE_NOT_CONNECTED;
	if( pKeystroke == NULL )
		return ERROR_INVALID_DATA;
	return ERROR_EMPTY;
}

#ifdef __cplusplus
}
#endif
