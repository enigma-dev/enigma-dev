# The only reason this exists is to make the flags clear. Damn MinGW.

default: release

LINKS := -lcomctl32 -lgdi32

obj/Debug:
	mkdir obj/Debug
obj/Release:
	mkdir obj/Release

Release: obj/Release
	windres.exe -i stupid-candy.rc -J rc -o obj\Release\stupid-candy.res -O coff 
	mingw32-g++.exe -Wall -fexceptions  -Os  -static -c main.cc -o obj\Release\main.o
	mingw32-g++.exe  -o ENIGMA.exe obj\Release\main.o obj\Release\stupid-candy.res -static $(LINKS) -Os --enable-auto-import -s  

Debug: obj/Debug
	mingw32-g++.exe -Wall -fexceptions  -g  -static -c main.cc -o obj\Debug\main.o
	mingw32-g++.exe  -o ENIGMA_debug.exe obj\Debug\main.o -static -g $(LINKS) --enable-auto-import

clean:
	rm obj/Debug/*
	rm obj/Release/*
