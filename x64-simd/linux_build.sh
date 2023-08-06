#!/bin/bash
echo "Building trader"

INCLUDES="-I ../../common/"
BUILD_DIR="./build/"
SRC_DIR=".."

BIN_NAME="learning"

SOURCES="$SRC_DIR/*.cpp"

DEBUG_FLAGS="-Wall -Werror -Wextra -Wdouble-promotion -Wformat -Wformat-overflow -Wno-unused-function -Wno-unused-variable -Wno-missing-field-initializers -Wunused -Wuninitialized -Wno-type-limits -Wno-implicit-fallthrough -Wno-unused-but-set-variable -Wno-sign-compare -Wno-format -Wno-double-promotion"

if [ ! -d "$BUILD_DIR" ]; then
  mkdir $BUILD_DIR
fi

pushd $BUILD_DIR
g++ $DEBUG_FLAGS -march=skylake -fdiagnostics-show-caret -g -ggdb -gdwarf -g3 -Og -O0 -p -fno-exceptions -fno-stack-protector -fno-rtti -save-temps $INCLUDES -nostdinc++ -D_GNU_SOURCE $SOURCES -o $BIN_NAME

  chmod +x $BIN_NAME
  cp ../../utils/.gdbinit .gdbinit
popd

