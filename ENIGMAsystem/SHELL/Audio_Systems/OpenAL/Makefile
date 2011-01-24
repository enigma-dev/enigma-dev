#Awesome Locally Generated Makefile

.eobjs_$(MODE)/as_basic.o: as_basic.cpp as_basic.h alure/include/AL/alure.h alure/config.h ../../../additional/al/include/alc.h ../../../additional/al/include/al.h
	g++ -c as_basic.cpp		-o .eobjs_$(MODE)/as_basic.o $(FLAGS)

.eobjs_$(MODE):
	-mkdir .eobjs_$(MODE)

Run: .eobjs_$(MODE) .eobjs_Run/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Run" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Run" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Run" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Run" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"

Debug: .eobjs_$(MODE) .eobjs_Debug/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Debug" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Debug" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Debug" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Debug" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"

Build: .eobjs_$(MODE) .eobjs_Build/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Build" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Build" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Build" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Build" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"

Release: .eobjs_$(MODE) .eobjs_Release/as_basic.o 
	cd alure  && $(MAKE) static DEST="../.eobjs_Release" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd dumb   && $(MAKE) static DEST="../.eobjs_Release" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ogg    && $(MAKE) static DEST="../.eobjs_Release" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd vorbis && $(MAKE) static DEST="../.eobjs_Release" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"


clean:
	-rm .eobjs*/*
	cd alure	&& $(MAKE) clean
	cd dumb		&& $(MAKE) clean
	cd ogg		&& $(MAKE) clean
	cd vorbis	&& $(MAKE) clean
