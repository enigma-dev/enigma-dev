#!/bin/sh

cd ../CompilerSource

echo "#define DIRECTORY_CHAR '/'
#define DIRECTORY_STR "/"

#define WINDOWS 0
#define MAXOSX 0
#define LINUX 1 > OS_Switchboard.h
"

g++ main.cpp -o compileEGMf.exe
mv -f "compileEGMf.exe" "../compileEGMf.exe"
cd ../ENIGMAsystem/SHELL

echo "#define ENIGMA_GS_OPENGL 1
#define ENIGMA_WS_XLIB 1 > API_Switchboard.h
"

cd Universal_System
g++ -c *.cpp
cd ../Platforms/windows
g++ -c *.cpp
cd ../../Graphics_Systems/OpenGL
g++ -c *.cpp

echo "Process Complete"

