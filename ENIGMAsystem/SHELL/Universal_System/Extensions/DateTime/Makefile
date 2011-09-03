all: extension
extension: .eobjs/$(COMPILEPATH)/datetime.o
.eobjs/$(COMPILEPATH)/datetime.o: date_time.cpp include.h 
	mkdir -p .eobjs/$(COMPILEPATH)/
	g++ -c date_time.cpp -o $@

