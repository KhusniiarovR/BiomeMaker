@echo off

cd /d "%~dp0"

where.exe cmake >nul 2>&1

where.exe g++ >nul 2>&1

set "SRC_DIR=%cd%"
set "BUILD_DIR=%SRC_DIR%\build\debug"

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

echo Generating CMake files...
cmake -G "MinGW Makefiles" -S "%SRC_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Debug

echo Building project...
cmake --build "%BUILD_DIR%"

echo Build complete!
echo Launching:
"%BUILD_DIR%\BiomeMaker.exe"

exit /b 0
