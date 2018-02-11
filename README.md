# XInput-Wrapper-For-UWP

**XInput wrapper for UWP** is a xinput-like interface for the UWP gamepad API.\
It provides the ability to use the xbox one controller capabilities ( including trigger vibration ) in a Win32 desktop application while keeping xinput1.3 compatibility for pre-Windows10 systems.

# How to use

You can get a pre-compiled version from [here](https://github.com/bdumesnil/XInput-Wrapper-For-UWP-Gamepads/releases).\
Include ``XInputWrapper.h`` in your code instead of the base ``xinput.h`` and copy ``XInputUWP.dll`` in your application directory ( ``XInputUWP64.dll`` for 64bits application )\
Add calls to ``XInputInit`` and ``XInputDestroy`` functions.

# How to build

Call ``project/GenerateSolution.bat`` to generate a VS2017 solution.\
Use ``MakeRedist.bat`` to automatically generate a release version of the library.

# Differences with xinput

Currently the wrapper only supports gamepads and doesn't support headset and battery functions.
The wrapper adds init and destroy functions.