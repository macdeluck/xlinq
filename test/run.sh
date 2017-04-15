#!/bin/bash
ROOT=${PWD}
OUT_LINUX=out/linux
cd $OUT_LINUX
ctest
cd $ROOT
read -n1 -r -p "Press any key to continue..." key