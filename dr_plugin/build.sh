#!/bin/bash

mkdir build
cd build
cmake -D CMAKE_C_COMPILER=/usr/bin/clang ..
make
cp *.so ..
