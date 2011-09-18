all: extension
extension: .eobjs/$(COMPILEPATH)/gme.o
.eobjs/$(COMPILEPATH)/gme.o: gme.cpp include.h ../../../Audio_Systems/audio_mandatory.h
	mkdir -p .eobjs/$(COMPILEPATH)/
	g++ -lgme -c gme.cpp -o $@
