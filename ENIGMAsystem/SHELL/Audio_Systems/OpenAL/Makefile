#Awesome Locally Generated Makefile

.eobjs_$(MODE)/as_basic.o: as_basic.cpp as_basic.h alure/include/AL/alure.h alure/config.h ../../../additional/al/include/alc.h ../../../additional/al/include/al.h
	g++ -c as_basic.cpp		-o .eobjs_$(MODE)/as_basic.o $(FLAGS)

mkObjDir:
	-mkdir .eobjs_$(MODE)

Run: mkObjDir .eobjs_Run/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Run" GFLAGS="$(FLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Run" GFLAGS="$(FLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Run" GFLAGS="$(FLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Run" GFLAGS="$(FLAGS)"

Debug: mkObjDir .eobjs_Debug/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Debug" GFLAGS="$(FLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Debug" GFLAGS="$(FLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Debug" GFLAGS="$(FLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Debug" GFLAGS="$(FLAGS)"

Build: mkObjDir .eobjs_Build/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Build" GFLAGS="$(FLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Build" GFLAGS="$(FLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Build" GFLAGS="$(FLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Build" GFLAGS="$(FLAGS)"

Release: mkObjDir .eobjs_Release/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Release" GFLAGS="$(FLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Release" GFLAGS="$(FLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Release" GFLAGS="$(FLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Release" GFLAGS="$(FLAGS)"


clean:
	-rm .eobjs*/*
	cd alure	&& $(MAKE) clean
	cd dumb		&& $(MAKE) clean
	cd ogg		&& $(MAKE) clean
	cd vorbis	&& $(MAKE) clean
