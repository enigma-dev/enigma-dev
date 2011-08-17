all: extension
extension: .eobjs/$(COMPILEPATH) .eobjs/$(COMPILEPATH)/paths.o
.eobjs/$(COMPILEPATH):
	mkdir -p .eobjs/$(COMPILEPATH)/
.eobjs/$(COMPILEPATH)/paths.o: paths.cpp
	g++ -c paths.cpp -o $@

