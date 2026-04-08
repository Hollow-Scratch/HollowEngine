#!/usr/bin/env bash

echo "=========================="
echo "1. Build Debug"
echo "2. Build Release"
echo "3. Build and Run Debug"
echo "4. Build and Run Release"
echo "5. Clean Debug"
echo "6. Clean Release"
echo "7. Clean All"
echo "=========================="

read -p "Choose option: " choice

RUN_AFTER_BUILD=0

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

elif [ "$choice" = "5" ]; then
  echo "Cleaning Debug..."
  rm -rf build/debug
  echo "Done."
  exit 0

elif [ "$choice" = "6" ]; then
  echo "Cleaning Release..."
  rm -rf build/release
  echo "Done."
  exit 0

elif [ "$choice" = "7" ]; then
  echo "Cleaning All..."
  rm -rf build
  echo "Done."
  exit 0

else
  echo "Invalid choice."
  exit 1
fi

# Configure (Ninja)
cmake -S . -B "$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE="$CONFIG" || exit 1

# Build
cmake --build "$BUILD_DIR" || exit 1

# Run (if selected)
if [ "$RUN_AFTER_BUILD" = "1" ]; then
  echo "Running..."
  "$BUILD_DIR/src/Game"
fi

echo "Done."
