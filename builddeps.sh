#!/bin/bash

set -e
set -x

rm -rf 3rdp/wasm 3rdp/native

git submodule update --init --recursive

# Native glog
cd 3rdp/glog
mkdir n
cd n
cmake -DCMAKE_INSTALL_PREFIX=../../native ..
make
make install
cd ..
rm -rf n
cd ../..

# WASM glog
cd 3rdp/glog
git apply ../patch/glog.patch
mkdir n
cd n
emconfigure cmake -DEMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES=ON \
  -DCMAKE_INSTALL_PREFIX=../../wasm ..
emmake make
make install
cd ..
rm -rf n
git reset --hard
cd ../..





