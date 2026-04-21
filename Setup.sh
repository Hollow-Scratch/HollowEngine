#!/usr/bin/env bash

echo "=========================="
echo "GCC"
echo "1.  Build Debug (All)"
echo "2.  Build Release (All)"
echo "3.  Build + Run Debug (Game)"
echo "4.  Build + Run Release (Game)"
echo "5.  Build + Run Debug (Editor)"
echo "6.  Build + Run Release (Editor)"
echo
echo "CLANG"
echo "7.  Build Debug (All)"
echo "8.  Build Release (All)"
echo "9.  Build + Run Debug (Game)"
echo "10. Build + Run Release (Game)"
echo "11. Build + Run Debug (Editor)"
echo "12. Build + Run Release (Editor)"
echo
echo "CLEAN"
echo "13. Clean Debug"
echo "14. Clean Release"
echo "15. Clean All"
echo "=========================="

read -p "Choose option: " choice

RUN_GAME=0
RUN_EDITOR=0
CONFIG=""
BUILD_DIR=""
COMPILER_C=""
COMPILER_CXX=""
GENERATOR="Ninja"

find_exe() {
    local name="$1"
    find "$BUILD_DIR" -type f -executable -name "$name" 2>/dev/null | head -n 1
}

case "$choice" in
1) CONFIG="Debug"; BUILD_DIR="build/debug-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++" ;;
2) CONFIG="Release"; BUILD_DIR="build/release-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++" ;;
3) CONFIG="Debug"; BUILD_DIR="build/debug-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++"; RUN_GAME=1 ;;
4) CONFIG="Release"; BUILD_DIR="build/release-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++"; RUN_GAME=1 ;;
5) CONFIG="Debug"; BUILD_DIR="build/debug-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++"; RUN_EDITOR=1 ;;
6) CONFIG="Release"; BUILD_DIR="build/release-gcc"; COMPILER_C="gcc"; COMPILER_CXX="g++"; RUN_EDITOR=1 ;;

7) CONFIG="Debug"; BUILD_DIR="build/debug-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++" ;;
8) CONFIG="Release"; BUILD_DIR="build/release-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++" ;;
9) CONFIG="Debug"; BUILD_DIR="build/debug-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++"; RUN_GAME=1 ;;
10) CONFIG="Release"; BUILD_DIR="build/release-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++"; RUN_GAME=1 ;;
11) CONFIG="Debug"; BUILD_DIR="build/debug-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++"; RUN_EDITOR=1 ;;
12) CONFIG="Release"; BUILD_DIR="build/release-clang"; COMPILER_C="clang"; COMPILER_CXX="clang++"; RUN_EDITOR=1 ;;

13) rm -rf build/debug-*; echo "Cleaned Debug"; exit 0 ;;
14) rm -rf build/release-*; echo "Cleaned Release"; exit 0 ;;
15) rm -rf build; echo "Cleaned All"; exit 0 ;;

*) echo "Invalid choice"; exit 1 ;;
esac

cmake -S . -B "$BUILD_DIR" \
  -G "$GENERATOR" \
  -DCMAKE_BUILD_TYPE="$CONFIG" \
  -DCMAKE_C_COMPILER="$COMPILER_C" \
  -DCMAKE_CXX_COMPILER="$COMPILER_CXX" || exit 1

# ✅ FIXED BUILD STEP
if [ "$RUN_GAME" = "1" ]; then
  cmake --build "$BUILD_DIR" --target Sandbox --parallel || exit 1
elif [ "$RUN_EDITOR" = "1" ]; then
  cmake --build "$BUILD_DIR" --target Editor --parallel || exit 1
else
  cmake --build "$BUILD_DIR" --parallel || exit 1
fi

if [ "$RUN_GAME" = "1" ]; then
  echo "Running Game..."
  exe=$(find_exe "Sandbox")
  [ -n "$exe" ] && "$exe" || echo "Game executable not found"
fi

if [ "$RUN_EDITOR" = "1" ]; then
  echo "Running Editor..."
  exe=$(find_exe "Editor")
  [ -n "$exe" ] && "$exe" || echo "Editor executable not found"
fi

echo "Done."