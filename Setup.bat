@echo off
setlocal

echo ==========================
echo 1. Build Debug
echo 2. Build Release
echo 3. Build and Run Debug
echo 4. Build and Run Release
echo 5. Clean Debug
echo 6. Clean Release
echo 7. Clean All
echo ==========================

set /p choice=Choose option: 

set RUN_AFTER_BUILD=0

if "%choice%"=="1" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug
    goto build
)

if "%choice%"=="2" (
    set CONFIG=Release
    set BUILD_DIR=build\release
    goto build
)

if "%choice%"=="3" (
    set CONFIG=Debug
    set BUILD_DIR=build\debug
    set RUN_AFTER_BUILD=1
    goto build
)

if "%choice%"=="4" (
    set CONFIG=Release
    set BUILD_DIR=build\release
    set RUN_AFTER_BUILD=1
    goto build
)

if "%choice%"=="5" (
    echo Cleaning Debug...
    rmdir /s /q build\debug
    echo Done.
    exit /b 0
)

if "%choice%"=="6" (
    echo Cleaning Release...
    rmdir /s /q build\release
    echo Done.
    exit /b 0
)

if "%choice%"=="7" (
    echo Cleaning All...
    rmdir /s /q build
    echo Done.
    exit /b 0
)

echo Invalid choice.
exit /b 1

:build
cmake -S . -B %BUILD_DIR% -G Ninja -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b %errorlevel%

cmake --build %BUILD_DIR%
if errorlevel 1 exit /b %errorlevel%

if "%RUN_AFTER_BUILD%"=="1" (
    echo Running...
    "%BUILD_DIR%\src\Game.exe"
)

echo Done.
exit /b 0
