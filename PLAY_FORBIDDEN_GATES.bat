@echo off
setlocal EnableExtensions

title The Forbidden Gates - Play Desktop

set "PROJECT=%~dp0TheForbiddenGates.uproject"

if not exist "%PROJECT%" (
  echo [ERROR] TheForbiddenGates.uproject was not found beside this BAT file.
  echo Keep PLAY_FORBIDDEN_GATES.bat in the project root folder.
  pause
  exit /b 1
)

rem ------------------------------------------------------------
rem 1) If a packaged Windows build already exists, run it.
rem ------------------------------------------------------------
for /r "%~dp0Builds\Windows" %%F in (*.exe) do (
  echo %%~nxF | findstr /i /v "CrashReport UnrealPrereq" >nul
  if not errorlevel 1 (
    echo Starting packaged game: %%F
    start "" "%%F"
    exit /b 0
  )
)

rem ------------------------------------------------------------
rem 2) Locate Unreal Engine 5.8.
rem You may also set UE_ROOT yourself before running this file.
rem ------------------------------------------------------------
if defined UE_ROOT (
  if exist "%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe" goto :UE_FOUND
)

for %%D in (
  "C:\Program Files\Epic Games\UE_5.8"
  "C:\Epic Games\UE_5.8"
  "D:\Program Files\Epic Games\UE_5.8"
  "D:\Epic Games\UE_5.8"
  "E:\Program Files\Epic Games\UE_5.8"
  "E:\Epic Games\UE_5.8"
) do (
  if exist "%%~D\Engine\Binaries\Win64\UnrealEditor.exe" (
    set "UE_ROOT=%%~D"
    goto :UE_FOUND
  )
)

echo.
echo [ERROR] Unreal Engine 5.8 was not found.
echo Install UE 5.8 using Epic Games Launcher, or set UE_ROOT to your UE_5.8 folder.
echo Example:
echo   set UE_ROOT=C:\Program Files\Epic Games\UE_5.8
echo.
pause
exit /b 1

:UE_FOUND
set "EDITOR=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe"
set "BUILD=%UE_ROOT%\Engine\Build\BatchFiles\Build.bat"

echo.
echo Unreal Engine:
echo   %UE_ROOT%
echo.
echo Project:
echo   %PROJECT%
echo.

rem ------------------------------------------------------------
rem 3) Compile the Editor target if the Unreal build script exists.
rem This requires the normal Unreal C++ prerequisites / Visual Studio.
rem ------------------------------------------------------------
if exist "%BUILD%" (
  echo Building TheForbiddenGatesEditor...
  call "%BUILD%" TheForbiddenGatesEditor Win64 Development -Project="%PROJECT%" -WaitMutex
  if errorlevel 1 (
    echo.
    echo [ERROR] Unreal could not compile the project.
    echo Install Visual Studio C++ tools / Unreal prerequisites and fix the first compile error shown above.
    pause
    exit /b 1
  )
)

rem ------------------------------------------------------------
rem 4) Launch directly in game mode.
rem Press Alt+F4 to close.
rem ------------------------------------------------------------
echo.
echo Launching The Forbidden Gates...
start "" "%EDITOR%" "%PROJECT%" -game -log

exit /b 0
