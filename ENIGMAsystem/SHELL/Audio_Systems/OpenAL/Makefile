#Awesome Locally Generated Makefile

$(OBJECTS)/as_basic.o: as_basic.cpp as_basic.h alure/include/AL/alure.h alure/config.h ../../../additional/al/include/alc.h ../../../additional/al/include/al.h
	g++ -c as_basic.cpp		-o $(OBJECTS)/as_basic.o $(FLAGS)

$(OBJECTS):
	-mkdir -p $(OBJECTS)

this: $(OBJECTS) $(OBJECTS)/as_basic.o 
	cd alure  && $(MAKE) static DEST="../$(OBJECTS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd dumb   && $(MAKE) static DEST="../$(OBJECTS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ogg    && $(MAKE) static DEST="../$(OBJECTS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd vorbis && $(MAKE) static DEST="../$(OBJECTS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"


clean:
	-rm .eobjs/*/*/*
	cd alure	&& $(MAKE) clean
	cd dumb		&& $(MAKE) clean
	cd ogg		&& $(MAKE) clean
	cd vorbis	&& $(MAKE) clean
