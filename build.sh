#!/bin/bash
# sorry Windows users ... don't know enough about Windows to help ya

BUILD_DIR="build"

# if the 'build' directory exists, remove it
if [ -d "$BUILD_DIR" ]; then
  rm -rf "$BUILD_DIR"
  echo "Existing build directory replaced"
fi

# make directory 'build'
mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || exit

# set release flags
CMAKE_FLAGS="-DCMAKE_CXX_FLAGS=-O3 -DNDEBUG -flto=auto -fuse-linker-plugin -march=native"

# if ninja available, use it, otherwise use make
if command -v ninja >/dev/null 2>&1; then
	echo "Ninja build system found. Using ninja."
	BUILD_COMMAND="ninja -j 0 -v"
	cmake -G Ninja "$CMAKE_FLAGS" ..
else
	BUILD_COMMAND="make -j"
	cmake "$CMAKE_FLAGS" ..
fi

# build
$BUILD_COMMAND

# give infos
printf "\nBuild completed successfully.\n"
printf "\nThe executable is in the 'build' directory\n\n"
