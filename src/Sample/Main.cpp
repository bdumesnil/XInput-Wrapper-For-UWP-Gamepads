#include <stdio.h>
#include <Windows.h>

#ifdef _WIN64
	#ifdef _DEBUG
		#define XINPUT_WRAPPER_UWP_DLL "XInputUWP64_d.dll"
	#else
		#define XINPUT_WRAPPER_UWP_DLL "XInputUWP64_r.dll"
	#endif
#else
	#ifdef _DEBUG
		#define XINPUT_WRAPPER_UWP_DLL "XInputUWP_d.dll"
	#else
		#define XINPUT_WRAPPER_UWP_DLL "XInputUWP_r.dll"
	#endif
#endif
#include "XInputWrapper.h"

int main( int /*argc*/, char* /*argv[]*/ )
{
	XinputVersion eLoadedVersion;
	XInputInit( XINPUT_VERSION_UWP, eLoadedVersion );
	switch( eLoadedVersion )
	{
		case XINPUT_VERSION_UWP:
			printf( "Using xinput UWP\n" );
			break;
		case XINPUT_VERSION_1_3:
			printf( "Using xinput 1.3\n" );
			break;
		case XINPUT_VERSION_NONE:
			printf( "Using dummy xinput\n" );
			break;
	}

	bool bGamepadConnected[ XUSER_MAX_COUNT ];
	for( int iGamepad = 0; iGamepad < XUSER_MAX_COUNT; ++iGamepad )
		bGamepadConnected[ iGamepad ] = false;

	bool bExit = false;
	while( !bExit )
	{
		for( int iGamepad = 0; iGamepad < XUSER_MAX_COUNT; ++iGamepad )
		{
			XINPUT_STATE oState = { 0 };
			DWORD iError = XInputGetState( iGamepad, &oState );
			if( iError == ERROR_DEVICE_NOT_CONNECTED && bGamepadConnected[ iGamepad ] )
			{
				printf( "Gamepad %d disconnected\n", iGamepad );
				bGamepadConnected[ iGamepad ] = false;
				continue;
			}
			else if( iError != ERROR_SUCCESS )
				continue;

			if( bGamepadConnected[ iGamepad ] == false )
			{
				printf( "Gamepad %d connected\n", iGamepad );
				bGamepadConnected[ iGamepad ] = true;
			}

			if( ( oState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) != 0 )
				printf( "Gamepad %d, button A\n", iGamepad );
			else if( ( oState.Gamepad.wButtons & XINPUT_GAMEPAD_B ) != 0 )
				printf( "Gamepad %d, button B\n", iGamepad );
			else if( ( oState.Gamepad.wButtons & XINPUT_GAMEPAD_X ) != 0 )
				printf( "Gamepad %d, button X\n", iGamepad );
			else if( ( oState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ) != 0 )
				printf( "Gamepad %d, button Y\n", iGamepad );
			else if( ( oState.Gamepad.wButtons & ( XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START ) ) != 0 )
				bExit = true;

			XINPUT_VIBRATION_EX oVibration = { 0 };
			float fLeftStick = (float)oState.Gamepad.sThumbLX / 32768.0f;
			if( fLeftStick < -0.1f )
				oVibration.wLeftMotorSpeed = ( WORD )( -fLeftStick * 65535.0f );
			else if( fLeftStick > 0.1f )
				oVibration.wRightMotorSpeed = ( WORD )( fLeftStick * 65535.0f );
			oVibration.wLeftTriggerSpeed = ( WORD )( ( ( float )oState.Gamepad.bLeftTrigger / 255.0f ) * 65535.0f );
			oVibration.wRightTriggerSpeed = ( WORD )( ( ( float )oState.Gamepad.bRightTrigger / 255.0f ) * 65535.0f );
			XInputSetStateEx( iGamepad, &oVibration );
		}
		Sleep( 10 );
	}

	for( int iGamepad = 0; iGamepad < XUSER_MAX_COUNT; ++iGamepad )
	{
		XINPUT_VIBRATION_EX oVibration = { 0 };
		XInputSetStateEx( iGamepad, &oVibration );
	}
	XInputDestroy();

	return 0;
}
