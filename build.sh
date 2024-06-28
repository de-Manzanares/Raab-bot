#!/bin/bash

BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
  rm -rf "$BUILD_DIR"
fi
mkdir "$BUILD_DIR"
echo "Existing build directory replaced"
cd "$BUILD_DIR"
cmake ..
make -j
echo "Build completed successfully."
echo "The executable is in the 'build' directory"
