cd ..
cd CompilerSource
echo #define DIRECTORY_CHAR '\\'^

#define DIRECTORY_STR "\\"^

^

#define WINDOWS 1^

#define MAXOSX 0^

#define LINUX 0 > OS_Switchboard.h
g++ main.cpp -o compileEGMf.exe
move /Y "compileEGMf.exe" "../compileEGMf.exe"
cd ..



cd ENIGMAsystem/SHELL
echo #define ENIGMA_GS_OPENGL 1^

#define ENIGMA_WS_WINDOWS 1 > API_Switchboard.h



cd Universal_System
g++ -c *.cpp
cd ..

cd Platforms/windows
g++ -c *.cpp
cd ../..

cd Graphics_Systems/OpenGL
g++ -c *.cpp

echo "Process Complete"