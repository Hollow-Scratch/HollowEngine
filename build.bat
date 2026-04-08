@echo off
setlocal

echo ==========================
echo 1. Build Debug
echo 2. Build Release
echo 3. Build and Run Debug
echo 4. Build and Run Release
echo ==========================
set /p choice=Choose option: 

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
    goto run_after_build
)

if "%choice%"=="4" (
    set CONFIG=Release
    set BUILD_DIR=build\release
    goto run_after_build
)

echo Invalid choice.
exit /b 1

:build
cmake -S . -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b %errorlevel%

cmake --build %BUILD_DIR% --parallel
if errorlevel 1 exit /b %errorlevel%

echo Done.
pause
exit /b 0

:run_after_build
cmake -S . -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b %errorlevel%

cmake --build %BUILD_DIR% --parallel
if errorlevel 1 exit /b %errorlevel%

"%BUILD_DIR%\my_app.exe"
pause
exit /b 0