#!/bin/bash

JOBS=`sysctl -n hw.ncpu`

set -e
set -x

#rm -rf 3rdp/wasm 3rdp/native
#git submodule update --init --recursive

# Native yaml-cpp
cd 3rdp/yaml-cpp
mkdir n
cd n
cmake -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF \
  -DYAML_CPP_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../native ..
make -j${JOBS}
make install
cd ..
rm -rf n
cd ../..

# WASM yaml-cpp
cd 3rdp/yaml-cpp
mkdir n
cd n
emconfigure cmake -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF \
-DYAML_CPP_BUILD_TESTS=OFF -DEMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES=ON -DCMAKE_INSTALL_PREFIX=../../wasm ..
emmake make -j${JOBS}
make install
cd ..
rm -rf n
cd ../..


# Native glog
cd 3rdp/glog
mkdir n
cd n
cmake -DCMAKE_INSTALL_PREFIX=../../native ..
make -j${JOBS}
make install
cd ..
rm -rf n
cd ../..

# WASM glog
cd 3rdp/glog
git apply ../patches/glog.patch
./autogen.sh
emconfigure  ./configure prefix=`pwd`/../wasm
emmake make -j${JOBS}
make install
git reset --hard
cd ../..





