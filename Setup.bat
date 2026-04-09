@echo off
setlocal

echo ==========================
echo MSVC
echo 1.  Build Debug
echo 2.  Build Release
echo 3.  Build and Run Debug
echo 4.  Build and Run Release
echo.
echo CLANG
echo 5.  Build Debug
echo 6.  Build Release
echo 7.  Build and Run Debug
echo 8.  Build and Run Release
echo.
echo CLEAN
echo 9.  Clean Debug
echo 10. Clean Release
echo 11. Clean All
echo ==========================

set /p choice=Choose option: 

set "RUN_AFTER_BUILD=0"
set "CONFIG="
set "BUILD_DIR="
set "COMPILER_C=cl"
set "COMPILER_CXX=cl"

if "%choice%"=="1"  goto msvc_debug
if "%choice%"=="2"  goto msvc_release
if "%choice%"=="3"  goto msvc_debug_run
if "%choice%"=="4"  goto msvc_release_run
if "%choice%"=="5"  goto clang_debug
if "%choice%"=="6"  goto clang_release
if "%choice%"=="7"  goto clang_debug_run
if "%choice%"=="8"  goto clang_release_run
if "%choice%"=="9"  goto clean_debug
if "%choice%"=="10" goto clean_release
if "%choice%"=="11" goto clean_all

echo Invalid choice.
exit /b 1

:msvc_debug
set "CONFIG=Debug"
set "BUILD_DIR=build\debug"
goto build

:msvc_release
set "CONFIG=Release"
set "BUILD_DIR=build\release"
goto build

:msvc_debug_run
set "CONFIG=Debug"
set "BUILD_DIR=build\debug"
set "RUN_AFTER_BUILD=1"
goto build

:msvc_release_run
set "CONFIG=Release"
set "BUILD_DIR=build\release"
set "RUN_AFTER_BUILD=1"
goto build

:clang_debug
set "CONFIG=Debug"
set "BUILD_DIR=build\debug-clang"
set "COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe"
set "COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe"
goto build

:clang_release
set "CONFIG=Release"
set "BUILD_DIR=build\release-clang"
set "COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe"
set "COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe"
goto build

:clang_debug_run
set "CONFIG=Debug"
set "BUILD_DIR=build\debug-clang"
set "COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe"
set "COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe"
set "RUN_AFTER_BUILD=1"
goto build

:clang_release_run
set "CONFIG=Release"
set "BUILD_DIR=build\release-clang"
set "COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe"
set "COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe"
set "RUN_AFTER_BUILD=1"
goto build

:clean_debug
echo Cleaning Debug...
rmdir /s /q build\debug 2>nul
rmdir /s /q build\debug-clang 2>nul
echo Done.
exit /b 0

:clean_release
echo Cleaning Release...
rmdir /s /q build\release 2>nul
rmdir /s /q build\release-clang 2>nul
echo Done.
exit /b 0

:clean_all
echo Cleaning All...
rmdir /s /q build 2>nul
echo Done.
exit /b 0

:build
rem Remove MSYS tools so native Windows CMake/Ninja/MSVC tools win
set "PATH=%PATH:C:\msys64\ucrt64\bin;=%"
set "PATH=%PATH:C:\msys64\usr\bin;=%"

rem Load MSVC environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%

rem Prefer native Windows CMake + Ninja
set "PATH=C:\Program Files\CMake\bin;C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%"

cmake -S . -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_C_COMPILER="%COMPILER_C%" -DCMAKE_CXX_COMPILER="%COMPILER_CXX%"
if errorlevel 1 exit /b %errorlevel%

cmake --build "%BUILD_DIR%" --parallel
if errorlevel 1 exit /b %errorlevel%

if "%RUN_AFTER_BUILD%"=="1" (
    echo Running...
    "%BUILD_DIR%\src\Game.exe"
)

echo Done.
exit /b 0