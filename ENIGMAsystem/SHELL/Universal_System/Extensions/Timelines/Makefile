all: extension
extension: .eobjs/$(COMPILEPATH) .eobjs/$(COMPILEPATH)/timelines.o
.eobjs/$(COMPILEPATH):
	mkdir -p .eobjs/$(COMPILEPATH)/
.eobjs/$(COMPILEPATH)/timelines.o: timelines.cpp
	g++ -c timelines.cpp -o $@

