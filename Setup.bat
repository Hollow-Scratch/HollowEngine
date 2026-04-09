@echo off
setlocal

echo ==========================
echo 1. Build Debug [MSVC]
echo 2. Build Release [MSVC]
echo 3. Build and Run Debug [MSVC]
echo 4. Build and Run Release [MSVC]
echo 5. Clean Debug
echo 6. Clean Release
echo 7. Clean All
echo 8. Build Debug [Clang]
echo 9. Build Release [Clang]
echo 10. Build and Run Debug [Clang]
echo 11. Build and Run Release [Clang]
echo ==========================

set /p choice=Choose option: 

set RUN_AFTER_BUILD=0
set COMPILER_C=cl
set COMPILER_CXX=cl

if "%choice%"=="1" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug
    set COMPILER_C=cl
    set COMPILER_CXX=cl
    goto build
)

if "%choice%"=="2" (
    set CONFIG=Release
    set BUILD_DIR=build\release
    set COMPILER_C=cl
    set COMPILER_CXX=cl
    goto build
)

if "%choice%"=="3" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug
    set COMPILER_C=cl
    set COMPILER_CXX=cl
    set RUN_AFTER_BUILD=1
    goto build
)

if "%choice%"=="4" (
    set CONFIG=Release
    set BUILD_DIR=build\release
    set COMPILER_C=cl
    set COMPILER_CXX=cl
    set RUN_AFTER_BUILD=1
    goto build
)

if "%choice%"=="5" (
    echo Cleaning Debug...
    rmdir /s /q build\debug 2>nul
    rmdir /s /q build\debug-clang 2>nul
    echo Done.
    exit /b 0
)

if "%choice%"=="6" (
    echo Cleaning Release...
    rmdir /s /q build\release 2>nul
    rmdir /s /q build\release-clang 2>nul
    echo Done.
    exit /b 0
)

if "%choice%"=="7" (
    echo Cleaning All...
    rmdir /s /q build 2>nul
    echo Done.
    exit /b 0
)

if "%choice%"=="8" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug-clang
    set COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe
    set COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe
    goto build
)

if "%choice%"=="9" (
    set CONFIG=Release
    set BUILD_DIR=build\release-clang
    set COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe
    set COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe
    goto build
)

if "%choice%"=="10" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug-clang
    set COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe
    set COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe
    set RUN_AFTER_BUILD=1
    goto build
)

if "%choice%"=="11" (
    set CONFIG=Release
    set BUILD_DIR=build\release-clang
    set COMPILER_C=C:\Program Files\LLVM\bin\clang-cl.exe
    set COMPILER_CXX=C:\Program Files\LLVM\bin\clang-cl.exe
    set RUN_AFTER_BUILD=1
    goto build
)

echo Invalid choice.
exit /b 1

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