
pushd %cd%
cd project
call GenerateSolution.bat nopause

set KEY_NAME=HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VS7
set VALUE_NAME=15.0
FOR /F "tokens=2*" %%A IN ('REG.exe query "%KEY_NAME%" /v "%VALUE_NAME%"') DO (set pInstallDir=%%B)
call "%pInstallDir%\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64

popd

devenv /rebuild "release|Win32" "project\vs2017\XInputWrapper.sln"
devenv /rebuild "release|x64" "project\vs2017\XInputWrapper.sln"

mkdir redist

cp -f bin/XInputUWP_r.dll redist/XInputUWP.dll
cp -f bin/XInputUWP64_r.dll redist/XInputUWP64.dll
cp -f bin/Sample64_r.exe redist/
cp -fR src/XInputWrapper/XInputWrapper.h redist/