#!/bin/sh
cd "${0%/*}"
g++ xicon.cpp ../../../../shared/libpng-util/libpng-util.cpp -o xicon -I/usr/local/include -L/usr/local/lib -lpng -std=c++17 -DBUILD_XICON
./xicon in.png > out.h
