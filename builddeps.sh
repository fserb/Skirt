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
git apply ../patches/glog.patch
./autogen.sh
emconfigure  ./configure prefix=`pwd`/../wasm
emmake make
make install
git reset --hard
cd ../..





