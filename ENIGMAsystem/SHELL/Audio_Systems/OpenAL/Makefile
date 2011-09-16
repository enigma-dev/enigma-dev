#Awesome Locally Generated Makefile

$(OBJECTS)/as_basic.o: as_basic.cpp as_basic.h alure/include/AL/alure.h alure/config.h ../../../additional/al/include/alc.h ../../../additional/al/include/al.h wrap_oal.h
	g++ -c as_basic.cpp		-o $(OBJECTS)/as_basic.o $(FLAGS)
$(OBJECTS)/wrap_oal.o: wrap_oal.cpp ../../../additional/al/include/alc.h ../../../additional/al/include/al.h wrap_oal.h
	g++ -c wrap_oal.cpp		-o $(OBJECTS)/wrap_oal.o $(FLAGS)

$(OBJECTS):
	-mkdir -p $(OBJECTS) || mkdir "$(OBJECTS)"

this: $(OBJECTS) $(OBJECTS)/as_basic.o $(OBJECTS)/wrap_oal.o 
	cd alure  && $(MAKE) static DEST="../.eobjs" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd ../Codecs &&$(MAKE) 


clean:
	-rm .eobjs/*/*/*/*
	cd alure && $(MAKE) clean
