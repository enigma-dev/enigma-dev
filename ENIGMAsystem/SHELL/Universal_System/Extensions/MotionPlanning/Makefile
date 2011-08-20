all: extension
extension: .eobjs/$(COMPILEPATH) .eobjs/$(COMPILEPATH)/mp.o
.eobjs/$(COMPILEPATH):
	mkdir -p .eobjs/$(COMPILEPATH)/
.eobjs/$(COMPILEPATH)/mp.o: mp.cpp
	g++ -c mp.cpp -o $@

