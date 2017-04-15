#!/bin/bash
ROOT=${PWD}
OUT_LINUX=out/linux
mkdir -p $OUT_LINUX
cd $OUT_LINUX
cmake $ROOT && cmake --build .
cd $ROOT