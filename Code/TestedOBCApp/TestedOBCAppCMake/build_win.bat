@echo off
setlocal

rem Ensure CMAKE_PATH is just the directory where cmake.exe is located
set CMAKE_PATH=C:\Program Files\CMake\bin

rem Define build directory for Windows
set BUILD_DIR_WINDOWS=WindowsRelease

rem Clean and build for Windows
if exist %BUILD_DIR_WINDOWS% rmdir /s /q %BUILD_DIR_WINDOWS%
mkdir %BUILD_DIR_WINDOWS%
pushd %BUILD_DIR_WINDOWS%

rem Configure and build for Windows
"%CMAKE_PATH%\cmake.exe" -G "Visual Studio 17 2022" -A x64 ..
"%CMAKE_PATH%\cmake.exe" --build . --config Release

popd
endlocal


