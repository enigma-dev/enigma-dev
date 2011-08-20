# The only reason this exists is to make the flags clear. Damn MinGW.

default: Release

LINKS := -lcomctl32 -lgdi32

obj/Debug:
	mkdir -p obj/Debug
obj/Release:
	mkdir -p obj/Release

obj/Release/eyaml.o: obj/Release ../settings-parse/eyaml.cpp
	mingw32-g++.exe -c ../settings-parse/eyaml.cpp -o obj/Release/eyaml.o

Release: obj/Release obj/Release/eyaml.o
	windres.exe -i stupid-candy.rc -J rc -o obj\Release\stupid-candy.res -O coff 
	mingw32-g++.exe -Wall -fexceptions  -Os  -static -c main.cc -o obj\Release\main.o
	mingw32-g++.exe  -o ENIGMA.exe obj\Release\main.o obj\Release\stupid-candy.res -static $(LINKS) obj/Release/eyaml.o -Os --enable-auto-import -s  

Debug: obj/Debug obj/Release/eyaml.o
	mingw32-g++.exe -Wall -fexceptions  -g  -static -c main.cc -o obj\Debug\main.o
	mingw32-g++.exe  -o ENIGMA_debug.exe obj\Debug\main.o -static -g $(LINKS) obj/Release/eyaml.o --enable-auto-import
    
clean:
	rm obj/Debug/*
	rm obj/Release/*
