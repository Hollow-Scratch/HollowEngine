@echo off

echo ==========================
echo GCC
echo 1.  Build Debug (All)
echo 2.  Build Release (All)
echo 3.  Build + Run Debug (Game)
echo 4.  Build + Run Release (Game)
echo 5.  Build + Run Debug (Editor)
echo 6.  Build + Run Release (Editor)
echo.
echo CLANG
echo 7.  Build Debug (All)
echo 8.  Build Release (All)
echo 9.  Build + Run Debug (Game)
echo 10. Build + Run Release (Game)
echo 11. Build + Run Debug (Editor)
echo 12. Build + Run Release (Editor)
echo.
echo CLEAN
echo 13. Clean Debug
echo 14. Clean Release
echo 15. Clean All
echo ==========================

set /p choice=Choose option: 

set RUN_GAME=0
set RUN_EDITOR=0
set CONFIG=
set BUILD_DIR=
set COMPILER_C=
set COMPILER_CXX=
set GENERATOR=Ninja

REM GCC
if "%choice%"=="1"  set CONFIG=Debug& set BUILD_DIR=build\debug-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++
if "%choice%"=="2"  set CONFIG=Release& set BUILD_DIR=build\release-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++
if "%choice%"=="3"  set CONFIG=Debug& set BUILD_DIR=build\debug-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++& set RUN_GAME=1
if "%choice%"=="4"  set CONFIG=Release& set BUILD_DIR=build\release-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++& set RUN_GAME=1
if "%choice%"=="5"  set CONFIG=Debug& set BUILD_DIR=build\debug-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++& set RUN_EDITOR=1
if "%choice%"=="6"  set CONFIG=Release& set BUILD_DIR=build\release-gcc& set COMPILER_C=gcc& set COMPILER_CXX=g++& set RUN_EDITOR=1

REM CLANG
if "%choice%"=="7"  set CONFIG=Debug& set BUILD_DIR=build\debug-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++
if "%choice%"=="8"  set CONFIG=Release& set BUILD_DIR=build\release-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++
if "%choice%"=="9"  set CONFIG=Debug& set BUILD_DIR=build\debug-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++& set RUN_GAME=1
if "%choice%"=="10" set CONFIG=Release& set BUILD_DIR=build\release-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++& set RUN_GAME=1
if "%choice%"=="11" set CONFIG=Debug& set BUILD_DIR=build\debug-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++& set RUN_EDITOR=1
if "%choice%"=="12" set CONFIG=Release& set BUILD_DIR=build\release-clang& set COMPILER_C=clang& set COMPILER_CXX=clang++& set RUN_EDITOR=1

REM CLEAN
if "%choice%"=="13" (
    rmdir /s /q build\debug-* 2>nul
    echo Cleaned Debug
    exit /b
)

if "%choice%"=="14" (
    rmdir /s /q build\release-* 2>nul
    echo Cleaned Release
    exit /b
)

if "%choice%"=="15" (
    rmdir /s /q build 2>nul
    echo Cleaned All
    exit /b
)

if "%CONFIG%"=="" (
    echo Invalid choice
    exit /b 1
)

REM Configure
cmake -S . -B "%BUILD_DIR%" -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_C_COMPILER=%COMPILER_C% -DCMAKE_CXX_COMPILER=%COMPILER_CXX%
if errorlevel 1 exit /b 1

REM Build
cmake --build "%BUILD_DIR%" --parallel
if errorlevel 1 exit /b 1

REM -------- FIND EXECUTABLES --------
set GAME_EXE=
set EDITOR_EXE=

for /r "%BUILD_DIR%" %%f in (Sandbox.exe) do (
    set GAME_EXE=%%f
    goto :found_game
)
:found_game

for /r "%BUILD_DIR%" %%f in (Editor.exe) do (
    set EDITOR_EXE=%%f
    goto :found_editor
)
:found_editor

REM -------- RUN --------
if "%RUN_GAME%"=="1" (
    echo Running Game...
    if defined GAME_EXE (
        "%GAME_EXE%"
    ) else (
        echo Game executable not found
    )
)

if "%RUN_EDITOR%"=="1" (
    echo Running Editor...
    if defined EDITOR_EXE (
        "%EDITOR_EXE%"
    ) else (
        echo Editor executable not found
    )
)

echo Done.