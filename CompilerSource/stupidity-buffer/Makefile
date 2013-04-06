# The only reason this exists is to make the flags clear. Damn MinGW.

default: Release

LINKS := -lcomctl32 -lgdi32
CXXFLAGS := -I..

obj/Debug:
	mkdir.exe -p obj/Debug
obj/Release:
	mkdir.exe -p obj/Release

obj/Release/eyaml.o: obj/Release ../settings-parse/eyaml.cpp
	g++.exe $(CXXFLAGS) -c ../settings-parse/eyaml.cpp -o obj/Release/eyaml.o

Release: obj/Release obj/Release/eyaml.o
	windres.exe -i stupid-candy.rc -J rc -o obj\Release\stupid-candy.res -O coff 
	$(CXX) $(CXXFLAGS) -Wall -fexceptions  -Os  -static -c main.cc -o obj\Release\main.o
	$(CXX) -o ENIGMA.exe obj\Release\main.o obj\Release\stupid-candy.res -static $(LINKS) obj/Release/eyaml.o -Os --enable-auto-import -s  

Debug: obj/Debug obj/Release/eyaml.o
	$(CXX) $(CXXFLAGS) -Wall -fexceptions  -g  -static -c main.cc -o obj\Debug\main.o
	$(CXX) -o ENIGMA_debug.exe obj\Debug\main.o -static -g $(LINKS) obj/Release/eyaml.o --enable-auto-import
    
clean:
	rm obj/Debug/*
	rm obj/Release/*

