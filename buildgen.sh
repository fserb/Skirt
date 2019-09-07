#!/bin/sh

set -e

rm -rf build/
cmake -G Ninja -B build/native .
emconfigure cmake -G Ninja -B build/wasm .

echo
echo "Build with: ninja"
