#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")
SWITCH_TO_MMAP=OFF

if [ "$1" = "--mmap" ]; then
    echo "Using memory mapping for reading file"
    SWITCH_TO_MMAP=ON
elif [ -n "$1" ]; then
    echo "Use --mmap for changing file reading method"
    exit 1;\
else
    echo "Using standart API for reading file"
fi

cmake -S . -B "$ROOTDIR/build" -DSWITCH_TO_MMAP=$SWITCH_TO_MMAP
cmake --build "$ROOTDIR/build"
