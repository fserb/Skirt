#!/bin/bash
shopt -s nullglob dotglob
set -e

exec > build.ninja

cat << EOF
ninja_required_version = 1.9.0
builddir = build
emcc = emcc
cc = clang++
ccflags = -O3 -std=c++14 -ffast-math
ldflags = -flto

rule ninjagen
  generator = true
  description = generating build.ninja
  command = ./ninjagen.sh

build build build/force: ninjagen ./ninjagen.sh

rule emcc
  description = emcc \$in
  depfile = \$out.d
  deps = gcc
  command = \$emcc \$ccflags -MMD -MF \$out.d --llvm-lto 3 \$
    -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \$
    -s MODULARIZE=1 -s EXPORT_ALL=1 -s EXPORT_ES6=1 -c \$in -o \$out

rule emlink
  description = emcc linking \$out
  command = \$emcc \$ldflags -flto \$in -o \$out

rule cc
  description = clang \$in
  depfile = \$out.d
  deps = gcc
  command = \$cc \$ccflags -MMD -MF \$out.d -flto \$
    -c \$in -o \$out

rule link
  description = clang linking \$out
  command = \$cc \$ldflags \$in -o \$out

EOF

BC_OBJS=""
OBJS=""
for file in src/**/*.cpp; do
  BASE=${file:4:$((${#file}-8))}
  BC_OBJS="build/$BASE.bc $BC_OBJS"
  OBJS="build/$BASE.o $OBJS"
  echo "build build/$BASE.bc: emcc $file"
  echo "build build/$BASE.o: cc $file"
done

cat << EOF

build bin/skirt.mjs | bin/skirt.wasm: emlink $BC_OBJS

build bin/skirt: link $OBJS

build wasm: phony bin/skirt.mjs
build native: phony bin/skirt
build all: phony native wasm

default all
EOF
