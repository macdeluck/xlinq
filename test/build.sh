#!/bin/bash
ROOT=${PWD}
OUT_LINUX=out/linux
rm -rf $OUT_LINUX
mkdir -p $OUT_LINUX
cd $OUT_LINUX
cmake $ROOT && cmake --build .
cd $ROOT