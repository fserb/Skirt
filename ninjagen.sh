#!/bin/bash
shopt -s nullglob dotglob
set -e

exec > .build.ninja

DEFS="-g"

CWD=`pwd`

cat << EOF
ninja_required_version = 1.9.0
builddir = build
emcc = emcc
cc = clang++
ccflags = -Isrc/ -O3 -std=c++14 -ffast-math -fdiagnostics-color=always
ldflags = -flto

rule ninjagen
  generator = true
  restat = true
  description = generating build.ninja
  command = ./ninjagen.sh

build build build/force build.ninja: ninjagen ./ninjagen.sh

rule emcc
  description = emcc \$in
  depfile = \$out.d
  deps = gcc
  command = \$emcc \$ccflags -MMD -MF \$out.d --llvm-lto 3 \$
    -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \$
    -I3rdp/wasm/include \$
    -s MODULARIZE=1 -s EXPORT_ALL=1 -s EXPORT_ES6=1 -c \$in -o \$out

rule emlink
  description = emcc linking \$out
  command = \$emcc \$ldflags \$
    -lpthread 3rdp/wasm/lib/libglog.dylib \$
    \$in -o \$out

rule cc
  description = clang \$in
  depfile = \$out.d
  deps = gcc
  command = \$cc \$ccflags -MMD -MF \$out.d -flto \$
    -I3rdp/native/include \$
    -c \$in -o \$out

rule link
  description = clang linking \$out
  command = \$cc \$ldflags \$
    -L/usr/local/opt/binutils/lib -lbfd -ldl \$
    -L3rdp/native/lib -lglog \$
    \$in -o \$out

EOF

BC_OBJS=""
OBJS=""
for file in src/**/*.cc; do
  BASE=${file:4:$((${#file}-7))}

  OBJS="build/$BASE.o $OBJS"
  echo "build build/$BASE.o: cc $file"

  if [[ "$BASE" == "3rdp/backward" ]]; then continue; fi

  BC_OBJS="build/$BASE.bc $BC_OBJS"
  echo "build build/$BASE.bc: emcc $file"
done

cat << EOF

build bin/skirt.mjs | bin/skirt.wasm: emlink $BC_OBJS

build bin/skirt: link $OBJS

build wasm: phony build bin/skirt.mjs
build native: phony build bin/skirt
build all: phony native wasm

default all
EOF

if ! cmp .build.ninja build.ninja > /dev/null 2>&1; then
  mv .build.ninja build.ninja
else
  rm .build.ninja
fi

