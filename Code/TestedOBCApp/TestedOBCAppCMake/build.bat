@echo off
setlocal

rem Ensure CMAKE_PATH is just the directory where cmake.exe is located
set CMAKE_PATH=C:\Program Files\CMake\bin

rem Define build directories
set BUILD_DIR_WINDOWS=WindowsRelease
set BUILD_DIR_LINUX=LinuxRelease

rem Clean and build for Windows
if exist %BUILD_DIR_WINDOWS% rmdir /s /q %BUILD_DIR_WINDOWS%
mkdir %BUILD_DIR_WINDOWS%
pushd %BUILD_DIR_WINDOWS%

rem Configure and build for Windows
"%CMAKE_PATH%\cmake.exe" -G "Visual Studio 17 2022" -A x64 ..
"%CMAKE_PATH%\cmake.exe" --build . --config Release
popd

rem Set the correct path to your Linux toolchain file
set TOOLCHAIN_PATH=C:\path\to\your\linux\toolchain\file.cmake

rem Clean and build for Linux
if exist %BUILD_DIR_LINUX% rmdir /s /q %BUILD_DIR_LINUX%
mkdir %BUILD_DIR_LINUX%
pushd %BUILD_DIR_LINUX%

rem Configure and build for Linux
"%CMAKE_PATH%\cmake.exe" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN_PATH% ..
"%CMAKE_PATH%\cmake.exe" --build .

popd
endlocal

