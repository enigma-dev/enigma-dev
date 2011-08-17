all: extension
extension: .eobjs/$(COMPILEPATH)/alarmcode.o
.eobjs/$(COMPILEPATH)/alarmcode.o: alarmcode.cpp implement.h include.h ../recast.h
	mkdir -p .eobjs/$(COMPILEPATH)/
	g++ -c alarmcode.cpp -o $@

