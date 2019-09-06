#!/bin/bash

set -e
set -x

rm -rf 3rdp/wasm 3rdp/native

git submodule update --init --recursive

# Native glog
cd 3rdp/glog
./autogen.sh
./configure prefix=`pwd`/../native
make
make install
cd ../..

# WASM glog
cd 3rdp/glog
git apply ../patch/glog.patch
./autogen.sh
emconfigure ./configure prefix=`pwd`/../wasm
emmake make
make install
git reset --hard
cd ../..



