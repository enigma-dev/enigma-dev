all: extension
extension: .eobjs/$(COMPILEPATH)/datastructures.o
.eobjs/$(COMPILEPATH)/datastructures.o: data_structures.cpp include.h 
	mkdir -p .eobjs/$(COMPILEPATH)/
	g++ -c data_structures.cpp -o $@

