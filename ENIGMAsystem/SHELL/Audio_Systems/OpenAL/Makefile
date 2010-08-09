#Awesome Locally Generated Makefile

.eobjs_$(MODE)/as_basic.o: as_basic.cpp alure/include/AL/alure.h alure/config.h ../../../additional/al/include/alc.h ../../../additional/al/include/al.h
	g++ -c as_basic.cpp		-o .eobjs_$(MODE)/as_basic.o $(FLAGS)

mkObjDir:
	-mkdir .eobjs_$(MODE)

Run: mkObjDir .eobjs_Run/as_basic.o 
	cd alure && $(MAKE) static DEST="../.eobjs_Run"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Run"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Run"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Run"

Debug: mkObjDir .eobjs_Debug/as_basic.o 
	cd alure && $(MAKE) static DEST="../.eobjs_Debug"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Debug"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Debug"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Debug"

Build: mkObjDir .eobjs_Build/as_basic.o 
	cd alure && $(MAKE) static DEST="../.eobjs_Build"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Build"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Build"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Build"

Release: mkObjDir .eobjs_Release/as_basic.o 
	cd alure && $(MAKE) static DEST="../.eobjs_Release"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Release"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Release"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Release"


clean:
	$(CREMOVE) .eobjs*$(SLASHC)*
	cd alure	&& make clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)"
	cd ogg		&& make clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)"
	cd vorbis	&& make clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)"
	cd dumb		&& make clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)"
