#Awesome Generated Makefile

.eobjs_$(MODE)/as_basic.o: as_basic.cpp alure/include/AL/alure.h
	g++ -c as_basic.cpp		-o .eobjs_$(MODE)/as_basic.o $(FLAGS)

mkObjDir:
	-mkdir .eobjs_$(MODE)

Run: mkObjDir .eobjs_Run/as_basic.o 
	cd alure && $(MAKE) avail DEST="../.eobjs_Run"
	cd ogg    && $(MAKE) avail DEST="../.eobjs_Run"
	cd vorbis && $(MAKE) avail DEST="../.eobjs_Run"

Debug: mkObjDir .eobjs_Debug/as_basic.o 
	cd alure && $(MAKE) avail DEST="../.eobjs_Debug"
	cd ogg    && $(MAKE) avail DEST="../.eobjs_Debug"
	cd vorbis && $(MAKE) avail DEST="../.eobjs_Debug"

Build: mkObjDir .eobjs_Build/as_basic.o 
	cd alure && $(MAKE) avail DEST="../.eobjs_Build"
	cd ogg    && $(MAKE) avail DEST="../.eobjs_Build"
	cd vorbis && $(MAKE) avail DEST="../.eobjs_Build"

Release: mkObjDir .eobjs_Release/as_basic.o 
	cd alure && $(MAKE) avail DEST="../.eobjs_Release"
	cd ogg    && $(MAKE) avail DEST="../.eobjs_Release"
	cd vorbis && $(MAKE) avail DEST="../.eobjs_Release"


clean:
	rm -f .eobjs*/*
