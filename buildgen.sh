#!/bin/sh

set -e

TYPE="Release"

cmake -G Ninja -B build/native -DCMAKE_BUILD_TYPE=${TYPE} .
emconfigure cmake -G Ninja -B build/wasm -DCMAKE_BUILD_TYPE=${TYPE} -DEMSCRIPTEN=ON .

echo
echo "${TYPE} build with: ninja"
