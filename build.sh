#!/bin/bash
# sorry Windows users ... don't know enough about Windows to help ya

BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
  rm -rf "$BUILD_DIR"
fi

mkdir "$BUILD_DIR"
echo "Existing build directory replaced"
cd "$BUILD_DIR"

CMAKE_FLAGS="-DCMAKE_CXX_FLAGS=-O3 -DNDEBUG -flto=auto -fuse-linker-plugin -march=native"

if command -v ninja >/dev/null 2>&1; then
	echo "Ninja build system found. Using ninja."
	BUILD_COMMAND="ninja -j 0 -v"
	cmake -G Ninja "$CMAKE_FLAGS" ..
else
	BUILD_COMMAND="make -j"
	cmake "$CMAKE_FLAGS" ..
fi

$BUILD_COMMAND

echo "Build completed successfully."
echo "The executable is in the 'build' directory"
