#!/usr/bin/env bash

echo "=========================="
echo "1. Build Debug"
echo "2. Build Release"
echo "3. Build and Run Debug"
echo "4. Build and Run Release"
echo "=========================="
read -p "Choose option: " choice

if [ "$choice" = "1" ]; then
  CONFIG="Debug"
  BUILD_DIR="build/debug"
elif [ "$choice" = "2" ]; then
  CONFIG="Release"
  BUILD_DIR="build/release"
elif [ "$choice" = "3" ]; then
  CONFIG="Debug"
  BUILD_DIR="build/debug"
  RUN_AFTER_BUILD=1
elif [ "$choice" = "4" ]; then
  CONFIG="Release"
  BUILD_DIR="build/release"
  RUN_AFTER_BUILD=1
else
  echo "Invalid choice."
  exit 1
fi

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
if [ $? -ne 0 ]; then
  exit $?
fi

cmake --build "$BUILD_DIR" --parallel
if [ $? -ne 0 ]; then
  exit $?
fi

if [ "$RUN_AFTER_BUILD" = "1" ]; then
  "$BUILD_DIR/my_app"
fi

echo "Done."
read -p "Press Enter to continue..."
exit 0
