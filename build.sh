#!/bin/bash

BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
  rm -rf "$BUILD_DIR"
fi

mkdir "$BUILD_DIR"
echo "Existing build directory replaced"
cd "$BUILD_DIR"

if command -v ninja >/dev/null 2>&1; then
	echo "Ninja build system found. Using ninja."
	BUILD_COMMAND="ninja -j 0 -v"
	cmake -G Ninja ..
else
	BUILD_COMMAND="make -j"
	cmake ..
fi

$BUILD_COMMAND

echo "Build completed successfully."
echo "The executable is in the 'build' directory"
