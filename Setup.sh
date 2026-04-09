#!/usr/bin/env bash

echo "=========================="
echo "GCC"
echo "1.  Build Debug"
echo "2.  Build Release"
echo "3.  Build and Run Debug"
echo "4.  Build and Run Release"
echo
echo "CLANG"
echo "5.  Build Debug"
echo "6.  Build Release"
echo "7.  Build and Run Debug"
echo "8.  Build and Run Release"
echo
echo "CLEAN"
echo "9.  Clean Debug"
echo "10. Clean Release"
echo "11. Clean All"
echo "=========================="

read -p "Choose option: " choice

RUN_AFTER_BUILD=0
CONFIG=""
BUILD_DIR=""
COMPILER_C="gcc"
COMPILER_CXX="g++"

case "$choice" in
  1)
    CONFIG="Debug"
    BUILD_DIR="build/debug"
    ;;

  2)
    CONFIG="Release"
    BUILD_DIR="build/release"
    ;;

  3)
    CONFIG="Debug"
    BUILD_DIR="build/debug"
    RUN_AFTER_BUILD=1
    ;;

  4)
    CONFIG="Release"
    BUILD_DIR="build/release"
    RUN_AFTER_BUILD=1
    ;;

  5)
    CONFIG="Debug"
    BUILD_DIR="build/debug-clang"
    COMPILER_C="clang"
    COMPILER_CXX="clang++"
    ;;

  6)
    CONFIG="Release"
    BUILD_DIR="build/release-clang"
    COMPILER_C="clang"
    COMPILER_CXX="clang++"
    ;;

  7)
    CONFIG="Debug"
    BUILD_DIR="build/debug-clang"
    COMPILER_C="clang"
    COMPILER_CXX="clang++"
    RUN_AFTER_BUILD=1
    ;;

  8)
    CONFIG="Release"
    BUILD_DIR="build/release-clang"
    COMPILER_C="clang"
    COMPILER_CXX="clang++"
    RUN_AFTER_BUILD=1
    ;;

  9)
    echo "Cleaning Debug..."
    rm -rf build/debug build/debug-clang
    echo "Done."
    exit 0
    ;;

  10)
    echo "Cleaning Release..."
    rm -rf build/release build/release-clang
    echo "Done."
    exit 0
    ;;

  11)
    echo "Cleaning All..."
    rm -rf build
    echo "Done."
    exit 0
    ;;

  *)
    echo "Invalid choice."
    exit 1
    ;;
esac

cmake -S . -B "$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE="$CONFIG" \
  -DCMAKE_C_COMPILER="$COMPILER_C" \
  -DCMAKE_CXX_COMPILER="$COMPILER_CXX" || exit 1

cmake --build "$BUILD_DIR" --parallel || exit 1

if [ "$RUN_AFTER_BUILD" = "1" ]; then
  echo "Running..."
  "$BUILD_DIR/src/Game"
fi

echo "Done."