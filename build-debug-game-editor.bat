@echo off
setlocal

set UE_PATH="C:\apps\unreal\UE_5.4"
set ENGINE_BUILD_PATH=%UE_PATH%\Engine\Build\BatchFiles

set PROJECT_ROOT=%cd%
for %%F in ("%PROJECT_ROOT%") do set PROJECT_NAME=%%~nF
set UPROJECT_PATH=%PROJECT_ROOT%\%PROJECT_NAME%.uproject

echo Building %PROJECT_NAME% in DebugGame Editor configuration...
cd /d %ENGINE_BUILD_PATH%
Build.bat -Target="%PROJECT_NAME%Editor Win64 DebugGame" -Project="%UPROJECT_PATH%" -WaitMutex -FromMsBuild

echo Build complete (if no errors above).
pause
endlocal
