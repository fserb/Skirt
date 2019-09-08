#!/bin/sh

set -e

git submodule update --init --recursive

TYPE="Release"

export LDFLAGS="-L/usr/local/opt/llvm/lib"
export CPPFLAGS="-I/usr/local/opt/llvm/include"

cmake -G Ninja -B build/native -DCMAKE_BUILD_TYPE=${TYPE} -DCMAKE_CXX_COMPILER=$(which clang++) .
emconfigure cmake -G Ninja -B build/wasm -DCMAKE_BUILD_TYPE=${TYPE} -DEMSCRIPTEN=ON .

echo
echo "${TYPE} build with: ninja -C build/<wasm|native>"
