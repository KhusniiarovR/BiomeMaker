@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist %VSWHERE% exit /b 1

for /f "usebackq tokens=* delims=" %%i in (`%VSWHERE% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set VSINSTALL=%%i
)
if not defined VSINSTALL exit /b 1

set VCVARSALL=%VSINSTALL%\VC\Auxiliary\Build\vcvarsall.bat
if not exist "%VCVARSALL%" exit /b 1

call "%VCVARSALL%" x64 >nul 2>&1

set "SRC_DIR=%cd%"
set "BUILD_DIR=%SRC_DIR%\build\debug"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cmake -G Ninja -S "%SRC_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --config Debug
if errorlevel 1 exit /b 1

echo Build complete

"%BUILD_DIR%\BiomeMaker.exe"

exit
